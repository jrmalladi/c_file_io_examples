# ============================================================
# Makefile for CS 240 File I/O Learning Examples
# ============================================================
#
# USAGE:
#   make          - Build the main program
#   make hw2_main - Build the main program
#   make clean    - Remove compiled files and output files
#   make run      - Build and run the program
#   make debug    - Build with debug symbols (for gdb/lldb)
#
# LEARNING POINTS:
# - CC: The C compiler to use (gcc or clang)
# - CFLAGS: Compiler flags
#   -Wall    : Enable all warnings
#   -Werror  : Treat warnings as errors
#   -std=c17 : Use C17 standard
#   -g       : Include debug information
# ============================================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -std=c17

# Debug flags (includes symbols for debugger)
DEBUG_FLAGS = -g

# Source files
SRCS = hw2.c hw2_main.c
OBJS = hw2.o hw2_main.o

# Output executable
TARGET = hw2_main

# Generated output files (for cleanup)
OUTPUT_FILES = history_2024.txt edey_report.txt

# ============================================================
# BUILD RULES
# ============================================================

# Default target: build the main program
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile hw2.c to object file
hw2.o: hw2.c hw2.h
	$(CC) $(CFLAGS) -c hw2.c

# Compile hw2_main.c to object file
hw2_main.o: hw2_main.c hw2.h
	$(CC) $(CFLAGS) -c hw2_main.c

# Build with debug symbols
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

# Build and run
run: $(TARGET)
	./$(TARGET)

# ============================================================
# CLEANUP
# ============================================================

# Remove compiled files and output files
clean:
	rm -f $(TARGET) $(OBJS) $(OUTPUT_FILES)
	rm -f *.o

# ============================================================
# HELP
# ============================================================

help:
	@echo "Available targets:"
	@echo "  make          - Build the program"
	@echo "  make hw2_main - Build the program"
	@echo "  make clean    - Remove compiled and output files"
	@echo "  make run      - Build and run the program"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make help     - Show this help message"

# Mark targets that don't create files
.PHONY: all clean run debug help
