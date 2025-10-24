# Quiz Game 🎮

A command-line quiz game written in C with advanced features including difficulty levels, error handling, and comprehensive history tracking. Perfect for demonstrating C programming skills including file I/O, data structures, and memory management.

## Features

-  **20 Multiple-Choice Questions**: Diverse topics including geography, science, history, math, and art
-  **Progressive Scoring**: Correct answers earn 1 point each
-  **50/50 Help System**: Use 'H' during gameplay for a hint (scores 0.5 points if correct)
-  **Comprehensive History**: All player scores saved with timestamps and question counts
-  **Leaderboard**: View top 5 players with rankings and statistics
-  **Colorful UI**: Terminal colors for enhanced visual experience
-  **Game Over System**: Wrong answer ends the game - keeps it challenging!
-  **Robust Error Handling**: Validates all user inputs
-  **Timestamps**: Track when scores were achieved
-  **Statistics**: See how many questions were answered correctly

##  Architecture

### Data Structures
- `Question`: Stores question text, 4 options, correct answer, and difficulty level
- `HistoryEntry`: Tracks player name, score, questions answered, and timestamp
- `Difficulty`: Enum for difficulty levels (Easy, Medium, Hard)

### Key Functions
- `loadQuestions()`: Reads questions from file with error handling
- `saveHistory()`: Appends game results with timestamps
- `validateName()`: Ensures valid player name input
- `validateAnswer()`: Checks answer format
- `getTimestamp()`: Generates current timestamp
- `compareScores()`: Custom comparator for leaderboard sorting

##  Files

```
├── main.c           # Source code (enhanced with comments)
├── questions.txt    # Question database (20 questions)
├── history.txt      # Score history with timestamps
└── README.md        # Documentation
```

## 🔨 Compilation

### Using GCC (Recommended)
```bash
gcc main.c -o quiz.exe
```

### Using Visual Studio Compiler
```bash
cl main.c
```

### Using Online C Compiler
- [OnlineGDB](https://www.onlinegdb.com/online_c_compiler)
- [Replit](https://replit.com/languages/c)

## Running the Game

```bash
quiz.exe
```

## How to Play

1. **Launch**: Run the compiled program
2. **Select**: Choose "1. Play Game" from the main menu
3. **Enter Name**: Type your name (letters and spaces only)
4. **Answer Questions**: 
   - Type **A, B, C, or D** for your answer
   - Type **H** for 50/50 help (reveals correct answer, worth 0.5 points)
5. **Stay Correct**: Answer correctly to continue
6. **Survive**: Get a wrong answer = Game Over!

## Menu Options

1. **Play Game** - Start a new quiz session
2. **Game History** - View all past scores with timestamps
3. **Leaderboard** - See top 5 players ranked by score
4. **Help** - Display detailed game instructions
5. **Exit** - Quit the application

## History Format

Each entry in `history.txt` contains:
```
PlayerName Score QuestionsAnswered Timestamp
```

Example:
```
Alice 15.5 18 2024-01-15_14-30
Bob 20.0 20 2024-01-15_15-45
```

Note: Timestamps are displayed nicely in the UI (spaces and colons), but stored with underscores and hyphens for parsing compatibility.

## Adding More Questions

Edit `questions.txt` with the following format:
```
Question text here
Option A
Option B
Option C
Option D
A
```

**Example:**
```
What is the capital of France?
Paris
London
Berlin
Madrid
A
```

Each question requires exactly 6 lines. The last line is the correct answer (A, B, C, or D).

## Requirements

- **Compiler**: GCC, Clang, or MSVC
- **Platform**: Windows, Linux, or macOS
- **Terminal**: Command Prompt, PowerShell, or Terminal

## Code Quality Highlights

### What Makes This Portfolio-Worthy:

✅ **Clean Architecture**: Well-organized data structures and functions  
✅ **Error Handling**: Comprehensive input validation and file operation checks  
✅ **Memory Safety**: Proper use of fixed-size buffers and bounds checking  
✅ **Code Documentation**: Inline comments explaining functionality  
✅ **Cross-Platform**: Works on Windows, Linux, and macOS  
✅ **User Experience**: Color-coded output, helpful error messages  
✅ **Data Persistence**: File-based storage with timestamp tracking  
✅ **Scalability**: Easy to add more questions or features  

## Sample Gameplay

```
=== Main Menu ===
1. Play Game
2. Game History
3. Leaderboard
4. Help
5. Exit
Choose an option: 1

=== Play Game ===
Enter your name (letters and spaces only, max 49 chars): Alice

Question 1/20
What is the capital of France?
A. Paris
B. London
C. Berlin
D. Madrid
Your answer (A/B/C/D, or H for 50/50 help): A
✓ CORRECT!
+1 point.

...
```

## 🎓 Skills Demonstrated

- **C Programming**: Pointers, arrays, structs, enums
- **File I/O**: Reading/writing text files
- **Error Handling**: Input validation and error messages
- **Data Structures**: Arrays, structs, custom comparators
- **Algorithms**: Sorting with qsort
- **Cross-Platform**: Conditional compilation
- **User Interface**: Menu-driven CLI application
- **Memory Management**: Static allocation, buffer management


