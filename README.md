# CS 240 File I/O Learning Examples

This project demonstrates C file I/O operations based on the CS 240 Homework 2 assignment.

## Project Structure

```
c_file_io_examples/
├── hw2.h           # Header file with constants and prototypes
├── hw2.c           # Implementation of file I/O functions
├── hw2_main.c      # Test program
├── game_data.txt   # Sample input data
├── Makefile        # Build configuration
├── README.md       # This file
└── .vscode/        # VS Code configuration
    ├── tasks.json
    ├── launch.json
    └── c_cpp_properties.json
```

## Key Concepts

### 1. File Operations

```c
// Opening a file for reading
FILE *fp = fopen("filename.txt", "r");
if (fp == NULL) {
    // Handle error - file doesn't exist or can't be read
}

// Opening a file for writing
FILE *fp = fopen("output.txt", "w");
if (fp == NULL) {
    // Handle error - can't create/write file
}

// Always close files when done!
fclose(fp);
```

### 2. Reading Formatted Data with fscanf()

```c
// Format: 2024-01-10|Z. Edey,Purdue#28,3,2,30.5
int year, month, day, points, assists, blocks;
float minutes;
char name[64], team[64];

// %d    = integer
// %63[^,] = string up to 63 chars, stopping at comma
// %f    = float

int items_read = fscanf(fp, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                        &year, &month, &day, name, team,
                        &points, &assists, &blocks, &minutes);

// fscanf returns number of items successfully read
if (items_read == 9) {
    // Successfully read all fields
}
```

### 3. Writing Formatted Data with fprintf()

```c
// Write to file with formatting
fprintf(fp_out, "Player: %s\n", player_name);
fprintf(fp_out, "Points: %d\n", points);
fprintf(fp_out, "Average: %.2f\n", average);  // 2 decimal places
fprintf(fp_out, "%02d-%02d\n", month, day);   // Zero-padded
```

### 4. Error Handling

Always check return values:
- `fopen()` returns NULL on failure
- `fscanf()` returns number of items read (check against expected)
- Use error codes to communicate failures

## Building and Running

### Command Line

```bash
# Navigate to project directory
cd ~/work/c_file_io_examples

# Build the program
make

# Run the program
./hw2_main

# Or build and run together
make run

# Build with debug symbols
make debug

# Clean up
make clean
```

### VS Code

1. Open the folder in VS Code:
   ```bash
   code ~/work/c_file_io_examples
   ```

2. Build: Press `Cmd+Shift+B` (Mac) or `Ctrl+Shift+B` (Linux/Windows)

3. Run: Use Terminal > Run Task > "Run"

4. Debug:
   - Set breakpoints by clicking left of line numbers
   - Press `F5` to start debugging
   - Use the debug panel to step through code

## Sample Input Format

```
yyyy-mm-dd|player_name,team#points,assists,blocks,minutes
```

Example:
```
2024-01-10|Z. Edey,Purdue#28,3,2,30.5
```

## Functions Implemented

| Function | Purpose |
|----------|---------|
| `generate_matches_history()` | Generate Purdue's match history for a year |
| `match_most_valuable_player()` | Find MVP of a specific match |
| `average_points_player()` | Calculate player's average points |
| `purdue_best_winning_match_score()` | Find best winning game score |
| `purdue_best_month()` | Find month with highest win rate |
| `generate_player_report()` | Generate comprehensive player report |

## Error Codes

| Code | Meaning |
|------|---------|
| `SUCCESS (0)` | Operation completed successfully |
| `FILE_READ_ERR (-1)` | Cannot open file for reading |
| `FILE_WRITE_ERR (-2)` | Cannot open file for writing |
| `BAD_RECORD (-3)` | Invalid data format |
| `BAD_DATE (-4)` | Invalid date values |
| `NO_DATA_POINTS (-5)` | No matching data found |

## Tips for Learning

1. **Start with fopen/fclose**: Make sure you understand basic file opening
2. **Practice fscanf**: Try different format strings
3. **Always check errors**: Every file operation can fail
4. **Close your files**: Memory leaks happen when you forget!
5. **Use the debugger**: Step through the code to see how data flows
