#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_QUESTIONS 100
#define MAX_TEXT_LEN 200
#define HISTORY_FILE "history.txt"
#define QUESTIONS_FILE "questions.txt"
#define MAX_HISTORY 100

// Color codes for terminal output
#define GREEN "\x1B[32m"
#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define YELLOW "\x1B[33m"
#define CYAN "\x1B[36m"
#define MAGENTA "\x1B[35m"
#define RESET "\x1B[0m"

// Difficulty levels
typedef enum {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
} Difficulty;

// Question structure
typedef struct {
    char question[MAX_TEXT_LEN];
    char options[4][MAX_TEXT_LEN];
    char correct;
    Difficulty difficulty;
} Question;

// History entry structure
typedef struct {
    char name[MAX_NAME_LEN];
    float score;
    int questionsAnswered;
    char timestamp[20];
} HistoryEntry;

// Global variables
Question questions[MAX_QUESTIONS];
HistoryEntry history[MAX_HISTORY];
int totalQuestions = 0;
int totalHistory = 0;

/**
 * Clears the terminal screen
 * Cross-platform implementation
 */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * Pauses execution and clears screen
 */
void pauseAndClear() {
    printf("\nPress ENTER to continue...");
    getchar();
    clearScreen();
}

/**
 * Gets current timestamp as string
 */
void getTimestamp(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d_%H-%M", timeinfo);
}

/**
 * Saves player history to file
 * @param name - Player name
 * @param score - Final score
 * @param questionsAnswered - Number of questions answered
 */
void saveHistory(const char* name, float score, int questionsAnswered) {
    FILE* file = fopen(HISTORY_FILE, "a");
    if (file == NULL) {
        printf(RED "Error: Could not open history file for writing!\n" RESET);
        return;
    }
    
    char timestamp[20];
    getTimestamp(timestamp, sizeof(timestamp));
    fprintf(file, "%s %.1f %d %s\n", name, score, questionsAnswered, timestamp);
    fclose(file);
}

/**
 * Loads history from file
 */
void loadHistory() {
    totalHistory = 0;
    FILE* file = fopen(HISTORY_FILE, "r");
    if (file == NULL) return;

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%49s %f %d %19s", 
                   history[totalHistory].name, 
                   &history[totalHistory].score,
                   &history[totalHistory].questionsAnswered,
                   history[totalHistory].timestamp) == 4) {
            totalHistory++;
            if (totalHistory >= MAX_HISTORY) break;
        }
    }
    fclose(file);
}

/**
 * Displays all game history
 */
void viewHistory() {
    clearScreen();
    printf(CYAN "=== Game History ===\n" RESET);
    loadHistory();
    
    if (totalHistory == 0) {
        printf("No history found.\n");
    } else {
        printf(MAGENTA "%-20s %-10s %-8s %-15s\n" RESET, "Player", "Score", "Q's", "Date");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < totalHistory; i++) {
            // Format timestamp for display (replace underscores and hyphens)
            char displayTime[20];
            strcpy(displayTime, history[i].timestamp);
            for (int j = 0; displayTime[j] != '\0'; j++) {
                if (displayTime[j] == '_') displayTime[j] = ' ';
                if (displayTime[j] == '-') {
                    if (j == 4 || j == 7) {
                        displayTime[j] = '-';
                    } else {
                        displayTime[j] = ':';
                    }
                }
            }
            
            printf("%-20s %-10.1f %-8d %-15s\n", 
                   history[i].name, 
                   history[i].score,
                   history[i].questionsAnswered,
                   displayTime);
        }
    }
    pauseAndClear();
}

/**
 * Comparison function for sorting scores
 */
int compareScores(const void* a, const void* b) {
    HistoryEntry* h1 = (HistoryEntry*)a;
    HistoryEntry* h2 = (HistoryEntry*)b;
    if (h2->score > h1->score) return 1;
    else if (h2->score < h1->score) return -1;
    else return 0;
}

/**
 * Displays top 5 players leaderboard
 */
void viewLeaderboard() {
    clearScreen();
    printf(CYAN "=== Leaderboard ===\n" RESET);
    loadHistory();

    if (totalHistory == 0) {
        printf("No players yet.\n");
    } else {
        qsort(history, totalHistory, sizeof(HistoryEntry), compareScores);
        int top = totalHistory < 5 ? totalHistory : 5;
        
        printf(MAGENTA "Rank  Player               Score    Questions\n" RESET);
        printf("----------------------------------------------\n");
        for (int i = 0; i < top; i++) {
            char medal = ' ';
            if (i == 0) medal = '🥇';
            else if (i == 1) medal = '🥈';
            else if (i == 2) medal = '🥉';
            
            printf("%d%c.  " BLUE "%-20s" RESET " %-8.1f %-8d\n", 
                   i + 1, medal, history[i].name, history[i].score, history[i].questionsAnswered);
        }
    }
    pauseAndClear();
}

/**
 * Displays help information
 */
void help() {
    clearScreen();
    printf(CYAN "=== Help ===\n" RESET);
    printf("1. " BLUE "Play Game" RESET ": Answer quiz questions. Wrong answer = game over!\n");
    printf("2. " BLUE "Game History" RESET ": View all past player scores with timestamps\n");
    printf("3. " BLUE "Leaderboard" RESET ": View top 5 players with highest scores\n");
    printf("4. " BLUE "Help" RESET ": Shows this help menu\n");
    printf("5. " BLUE "Exit" RESET ": Quit the application\n\n");
    printf(YELLOW "Game Rules:\n" RESET);
    printf("• Correct answer: +1 point\n");
    printf("• Use 'H' for 50/50 help: reveals answer (scores 0.5 points)\n");
    printf("• One wrong answer ends the game\n");
    printf("• Try to answer all 20 questions!\n");
    pauseAndClear();
}

/**
 * Loads questions from file
 * @return Number of questions loaded, 0 on error
 */
int loadQuestions() {
    FILE* file = fopen(QUESTIONS_FILE, "r");
    if (file == NULL) {
        printf(RED "Error: Could not open questions file!\n" RESET);
        return 0;
    }
    
    totalQuestions = 0;
    while (fgets(questions[totalQuestions].question, MAX_TEXT_LEN, file)) {
        // Remove newline
        questions[totalQuestions].question[strcspn(questions[totalQuestions].question, "\n")] = 0;
        
        // Read 4 options
        for (int i = 0; i < 4; i++) {
            if (!fgets(questions[totalQuestions].options[i], MAX_TEXT_LEN, file)) {
                fclose(file);
                return totalQuestions;
            }
            questions[totalQuestions].options[i][strcspn(questions[totalQuestions].options[i], "\n")] = 0;
        }
        
        // Read correct answer
        char correctLine[MAX_TEXT_LEN];
        if (!fgets(correctLine, MAX_TEXT_LEN, file)) {
            fclose(file);
            return totalQuestions;
        }
        questions[totalQuestions].correct = toupper(correctLine[0]);
        
        // Default difficulty (can be enhanced later)
        questions[totalQuestions].difficulty = MEDIUM;
        
        totalQuestions++;
        if (totalQuestions >= MAX_QUESTIONS) break;
    }
    
    fclose(file);
    return totalQuestions;
}

/**
 * Validates player name input
 * @param name - Name to validate
 * @return 1 if valid, 0 otherwise
 */
int validateName(const char* name) {
    if (strlen(name) == 0 || strlen(name) >= MAX_NAME_LEN) {
        return 0;
    }
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

/**
 * Validates answer input
 * @param answer - Answer to validate
 * @return 1 if valid, 0 otherwise
 */
int validateAnswer(char answer) {
    char upper = toupper(answer);
    return (upper == 'A' || upper == 'B' || upper == 'C' || upper == 'D' || upper == 'H');
}

/**
 * Main game loop
 */
void playGame() {
    clearScreen();
    printf(CYAN "=== Play Game ===\n" RESET);

    char name[MAX_NAME_LEN];
    int valid = 0;

    // Input and validate name with improved feedback
    while (!valid) {
        printf("Enter your name (letters and spaces only, max %d chars): ", MAX_NAME_LEN - 1);
        
        if (fgets(name, sizeof(name), stdin) == NULL) {
            printf(RED "Error reading input!\n" RESET);
            pauseAndClear();
            return;
        }
        
        // Remove newline
        name[strcspn(name, "\n")] = 0;
        
        if (validateName(name)) {
            valid = 1;
        } else {
            printf(RED "Invalid input. Use letters and spaces only.\n" RESET);
        }
    }

    float score = 0;
    int questionsAnswered = 0;
    
    for (int i = 0; i < totalQuestions; i++) {
        printf("\n" YELLOW "Question %d/%d" RESET "\n", i + 1, totalQuestions);
        printf(YELLOW "%s" RESET "\n", questions[i].question);
        
        for (int j = 0; j < 4; j++) {
            printf("%c. " BLUE "%s" RESET "\n", 'A' + j, questions[i].options[j]);
        }
        
        printf("Your answer (A/B/C/D, or H for 50/50 help): ");
        char answer;
        
        // Improved input validation
        if (scanf(" %c", &answer) != 1) {
            printf(RED "Invalid input!\n" RESET);
            while (getchar() != '\n');
            pauseAndClear();
            return;
        }
        getchar();

        if (!validateAnswer(answer)) {
            printf(RED "Invalid answer. Please enter A, B, C, D, or H.\n" RESET);
            i--; // Repeat question
            continue;
        }

        if (answer == 'H' || answer == 'h') {
            printf(MAGENTA "50/50 Help: The correct answer is %c.\n" RESET, questions[i].correct);
            printf(MAGENTA "You now get 0.5 points if correct.\n" RESET);
            printf("Your answer (A/B/C/D): ");
            
            if (scanf(" %c", &answer) != 1) {
                printf(RED "Invalid input!\n" RESET);
                while (getchar() != '\n');
                i--; // Repeat question
                continue;
            }
            getchar();
            
            // Validate the answer after help
            if (toupper(answer) != 'A' && toupper(answer) != 'B' && 
                toupper(answer) != 'C' && toupper(answer) != 'D') {
                printf(RED "Invalid answer. Please enter A, B, C, or D.\n" RESET);
                i--; // Repeat question
                continue;
            }
            
            if (toupper(answer) == questions[i].correct) {
                printf(GREEN "✓ CORRECT!" RESET "\n");
                printf(GREEN "+0.5 points.\n" RESET);
                score += 0.5f;
                questionsAnswered++;
            } else {
                printf(RED "✗ Wrong! The correct answer was %c.\n" RESET, questions[i].correct);
                printf(RED "\nGame Over!" RESET " You scored %.1f points (%d/%d questions correct).\n", 
                       score, questionsAnswered, i + 1);
                saveHistory(name, score, questionsAnswered);
                pauseAndClear();
                return;
            }
        } else {
            if (toupper(answer) == questions[i].correct) {
                printf(GREEN "✓ CORRECT!" RESET "\n");
                printf(GREEN "+1 point.\n" RESET);
                score += 1.0f;
                questionsAnswered++;
            } else {
                printf(RED "✗ Wrong! The correct answer was %c.\n" RESET, questions[i].correct);
                printf(RED "\nGame Over!" RESET " You scored %.1f points (%d/%d questions correct).\n", 
                       score, questionsAnswered, i + 1);
                saveHistory(name, score, questionsAnswered);
                pauseAndClear();
                return;
            }
        }
    }

    printf("\n" GREEN "🎉 Congratulations!" RESET " You finished all questions!\n");
    printf("Total score: " BLUE "%.1f points" RESET " (%d/%d questions correct)\n", 
           score, questionsAnswered, totalQuestions);
    saveHistory(name, score, questionsAnswered);
    pauseAndClear();
}

/**
 * Displays main menu and handles user choices
 */
void showMainMenu() {
    int choice;
    do {
        printf(CYAN "=== Main Menu ===\n" RESET);
        printf("1. " BLUE "Play Game" RESET "\n");
        printf("2. " BLUE "Game History" RESET "\n");
        printf("3. " BLUE "Leaderboard" RESET "\n");
        printf("4. " BLUE "Help" RESET "\n");
        printf("5. " BLUE "Exit" RESET "\n");
        printf("Choose an option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf(RED "Invalid input! " RESET);
            while (getchar() != '\n');
            printf("Please enter a number.\n");
            printf("Redirecting to Help...\n");
            help();
            continue;
        }
        getchar();

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                viewHistory();
                break;
            case 3:
                viewLeaderboard();
                break;
            case 4:
                help();
                break;
            case 5:
                printf("Exiting... Goodbye!\n");
                break;
            default:
                printf(RED "Invalid option! " RESET);
                printf("Redirecting to Help...\n");
                help();
        }
    } while (choice != 5);
}

/**
 * Main entry point
 */
int main() {
    clearScreen();
    printf(CYAN "Welcome to the Quiz Game!\n" RESET);
    printf(YELLOW "Loading questions..." RESET "\n\n");
    
    if (loadQuestions() == 0) {
        printf(RED "Error: No questions loaded. Please check '%s' file.\n" RESET, QUESTIONS_FILE);
        printf("Press ENTER to exit...");
        getchar();
        return 1;
    }
    
    printf(GREEN "✓ Loaded %d questions successfully!\n\n" RESET, totalQuestions);
    pauseAndClear();
    
    showMainMenu();
    return 0;
}
