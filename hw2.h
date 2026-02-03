/*
 * hw2.h - Header file for CS 240 File I/O Examples
 *
 * This file contains:
 * - Error code definitions
 * - Constants
 * - Function prototypes
 *
 * Learning Concepts:
 * - Header guards (#ifndef/#define/#endif)
 * - Macro constants (#define)
 * - Function prototypes
 */

#ifndef HW2_H
#define HW2_H

/* ========== ERROR CODES ========== */
/* These are return values used to indicate success or specific errors */

#define SUCCESS         0   /* Operation completed successfully */
#define FILE_READ_ERR  -1   /* Could not open file for reading */
#define FILE_WRITE_ERR -2   /* Could not open file for writing */
#define BAD_RECORD     -3   /* Invalid data format in record */
#define BAD_DATE       -4   /* Invalid date (month 1-12, day 1-30, year > 0) */
#define NO_DATA_POINTS -5   /* No matching data found */

/* ========== CONSTANTS ========== */
#define MAX_NAME_LENGTH 64  /* Maximum length for player/team names */

/* ========== FUNCTION PROTOTYPES ========== */

/*
 * generate_matches_history
 *
 * Purpose: Read game data and generate a summary of Purdue's matches for a year
 *
 * Parameters:
 *   in_file  - Path to input data file
 *   year     - Year to filter matches (e.g., 2024)
 *   out_file - Path to output file
 *
 * Returns:
 *   SUCCESS on success, or appropriate error code
 *
 * Output format:
 *   <Year>
 *   mm-dd:Purdue(<score>)-<Opponent>(<score>)
 *   ...
 *   Record: <wins>W-<losses>L
 */
int generate_matches_history(char *in_file, int year, char *out_file);

/*
 * match_most_valuable_player
 *
 * Purpose: Find the MVP of a specific match and return their combined score
 *
 * Parameters:
 *   in_file - Path to input data file
 *   year    - Year of the match
 *   month   - Month of the match (1-12)
 *   day     - Day of the match (1-30)
 *
 * Returns:
 *   Combined score of MVP (double), or error code (cast to double)
 *   Combined score = points + 1.5*assists + 2*blocks + 0.2*minutes
 */
double match_most_valuable_player(char *in_file, int year, int month, int day);

/*
 * average_points_player
 *
 * Purpose: Calculate the average points per game for a specific player
 *
 * Parameters:
 *   in_file     - Path to input data file
 *   player_name - Name of the player to search for
 *
 * Returns:
 *   Average points (double), or error code (cast to double)
 */
double average_points_player(char *in_file, char *player_name);

/*
 * purdue_best_winning_match_score
 *
 * Purpose: Find Purdue's best winning game in a specific month/year
 *          "Best" = largest score difference where Purdue wins
 *
 * Parameters:
 *   in_file - Path to input data file
 *   year    - Year to search
 *   month   - Month to search (1-12)
 *
 * Returns:
 *   Purdue's total score in best winning game, or error code
 */
int purdue_best_winning_match_score(char *in_file, int year, int month);

/*
 * purdue_best_month
 *
 * Purpose: Find the month (1-12) with Purdue's highest win rate
 *
 * Parameters:
 *   in_file - Path to input data file
 *
 * Returns:
 *   Month number (1-12), or error code
 */
int purdue_best_month(char *in_file);

/*
 * generate_player_report
 *
 * Purpose: Generate a comprehensive statistics report for a Purdue player
 *
 * Parameters:
 *   in_file     - Path to input data file
 *   player_name - Name of the Purdue player
 *   out_file    - Path to output report file
 *
 * Returns:
 *   SUCCESS on success, or appropriate error code
 */
int generate_player_report(char *in_file, char *player_name, char *out_file);

#endif /* HW2_H */
