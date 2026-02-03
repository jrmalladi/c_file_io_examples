/*
 * hw2_main.c - Test Program for File I/O Examples
 *
 * This program demonstrates how to test each function
 * and shows expected outputs.
 *
 * COMPILE: make hw2_main
 * RUN:     ./hw2_main
 */

#include <stdio.h>
#include "hw2.h"

/*
 * Helper function to print error codes in human-readable form
 */
void print_result_code(int code) {
    switch (code) {
        case SUCCESS:
            printf("SUCCESS\n");
            break;
        case FILE_READ_ERR:
            printf("ERROR: FILE_READ_ERR\n");
            break;
        case FILE_WRITE_ERR:
            printf("ERROR: FILE_WRITE_ERR\n");
            break;
        case BAD_RECORD:
            printf("ERROR: BAD_RECORD\n");
            break;
        case BAD_DATE:
            printf("ERROR: BAD_DATE\n");
            break;
        case NO_DATA_POINTS:
            printf("ERROR: NO_DATA_POINTS\n");
            break;
        default:
            printf("UNKNOWN CODE: %d\n", code);
    }
}

/*
 * Helper function to print the contents of a file
 */
void print_file_contents(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", filename);
        return;
    }

    printf("\n--- Contents of %s ---\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    printf("--- End of file ---\n\n");

    fclose(fp);
}

int main() {
    int result;
    double dbl_result;

    printf("============================================\n");
    printf("   CS 240 File I/O Learning Examples\n");
    printf("============================================\n\n");

    /*
     * TEST 1: generate_matches_history
     */
    printf("=== TEST 1: generate_matches_history ===\n");
    printf("Reading game_data.txt, filtering year 2024...\n");

    result = generate_matches_history("game_data.txt", 2024, "history_2024.txt");
    printf("Result: ");
    print_result_code(result);

    if (result == SUCCESS) {
        print_file_contents("history_2024.txt");
    }

    /*
     * TEST 2: match_most_valuable_player
     */
    printf("=== TEST 2: match_most_valuable_player ===\n");
    printf("Finding MVP for match on 2024-01-10...\n");
    printf("Formula: points + 1.5*assists + 2*blocks + 0.2*minutes\n\n");

    dbl_result = match_most_valuable_player("game_data.txt", 2024, 1, 10);
    if (dbl_result >= 0) {
        printf("MVP Combined Score: %.2f\n\n", dbl_result);
    } else {
        printf("Result: ");
        print_result_code((int)dbl_result);
    }

    /*
     * TEST 3: average_points_player
     */
    printf("=== TEST 3: average_points_player ===\n");
    printf("Calculating average points for 'Z. Edey'...\n");

    dbl_result = average_points_player("game_data.txt", "Z. Edey");
    if (dbl_result >= 0) {
        printf("Average Points: %.2f\n\n", dbl_result);
    } else {
        printf("Result: ");
        print_result_code((int)dbl_result);
    }

    /*
     * TEST 4: purdue_best_winning_match_score
     */
    printf("=== TEST 4: purdue_best_winning_match_score ===\n");
    printf("Finding best winning match in January 2024...\n");

    result = purdue_best_winning_match_score("game_data.txt", 2024, 1);
    if (result >= 0) {
        printf("Best Winning Match Score: %d\n\n", result);
    } else {
        printf("Result: ");
        print_result_code(result);
    }

    /*
     * TEST 5: purdue_best_month
     */
    printf("=== TEST 5: purdue_best_month ===\n");
    printf("Finding Purdue's best month (highest win rate)...\n");

    result = purdue_best_month("game_data.txt");
    if (result >= 1 && result <= 12) {
        printf("Best Month: %d\n\n", result);
    } else {
        printf("Result: ");
        print_result_code(result);
    }

    /*
     * TEST 6: generate_player_report
     */
    printf("=== TEST 6: generate_player_report ===\n");
    printf("Generating report for 'Z. Edey'...\n");

    result = generate_player_report("game_data.txt", "Z. Edey", "edey_report.txt");
    printf("Result: ");
    print_result_code(result);

    if (result == SUCCESS) {
        print_file_contents("edey_report.txt");
    }

    /*
     * TEST 7: Error handling - file not found
     */
    printf("=== TEST 7: Error Handling (File Not Found) ===\n");
    printf("Trying to open non-existent file...\n");

    result = generate_matches_history("nonexistent.txt", 2024, "output.txt");
    printf("Result: ");
    print_result_code(result);
    printf("\n");

    /*
     * TEST 8: Error handling - bad date
     */
    printf("=== TEST 8: Error Handling (Invalid Date) ===\n");
    printf("Trying to find MVP for invalid date (month=13)...\n");

    dbl_result = match_most_valuable_player("game_data.txt", 2024, 13, 1);
    printf("Result: ");
    print_result_code((int)dbl_result);
    printf("\n");

    /*
     * TEST 9: Error handling - player not found
     */
    printf("=== TEST 9: Error Handling (Player Not Found) ===\n");
    printf("Looking for non-existent player 'J. Doe'...\n");

    dbl_result = average_points_player("game_data.txt", "J. Doe");
    printf("Result: ");
    print_result_code((int)dbl_result);
    printf("\n");

    printf("============================================\n");
    printf("           All Tests Completed!\n");
    printf("============================================\n");

    return 0;
}
