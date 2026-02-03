/*
 * hw2.c - Implementation of File I/O Functions
 *
 * CS 240 File Operations Learning Examples
 *
 * KEY CONCEPTS DEMONSTRATED:
 * 1. fopen()  - Opening files for reading ("r") or writing ("w")
 * 2. fscanf() - Reading formatted data from files
 * 3. fprintf()- Writing formatted data to files
 * 4. fclose() - Closing files (ALWAYS do this!)
 * 5. Error handling - Checking return values
 * 6. String parsing with format specifiers
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hw2.h"

/* ============================================================
 * HELPER FUNCTION: Validate a date
 * ============================================================
 * Checks if year > 0, month in [1,12], day in [1,30]
 */
static int is_valid_date(int year, int month, int day) {
    return (year > 0 && month >= 1 && month <= 12 && day >= 1 && day <= 30);
}

/* ============================================================
 * FUNCTION: generate_matches_history
 * ============================================================
 *
 * LEARNING POINTS:
 * - Opening files with fopen() and checking for NULL
 * - Using fscanf() with complex format strings
 * - Tracking state while reading (current date, scores)
 * - Writing formatted output with fprintf()
 */
int generate_matches_history(char *in_file, int year, char *out_file) {
    FILE *fp_in = NULL;
    FILE *fp_out = NULL;

    /* Validate year parameter */
    if (year <= 0) {
        return BAD_DATE;
    }

    /*
     * STEP 1: Open input file for reading
     *
     * fopen() returns NULL if the file cannot be opened
     * Always check this! Common reasons for failure:
     * - File doesn't exist
     * - No read permissions
     * - Path is wrong
     */
    fp_in = fopen(in_file, "r");
    if (fp_in == NULL) {
        return FILE_READ_ERR;
    }

    /*
     * STEP 2: Open output file for writing
     *
     * "w" mode creates the file if it doesn't exist
     * and truncates (empties) it if it does exist
     */
    fp_out = fopen(out_file, "w");
    if (fp_out == NULL) {
        fclose(fp_in);  /* Don't forget to close the input file! */
        return FILE_WRITE_ERR;
    }

    /*
     * STEP 3: Read and process data
     *
     * FILE FORMAT: yyyy-mm-dd|player_name,team#points,assists,blocks,minutes
     *
     * fscanf() format specifiers used:
     *   %d    - integer
     *   %63[^,] - string up to 63 chars, stopping at comma
     *   %f    - float
     *   %*c   - read and discard one character (like '|' or '-')
     *
     * Note: %63[^,] reads up to 63 characters until a comma is found
     *       This prevents buffer overflow (MAX_NAME_LENGTH is 64)
     */

    int r_year, r_month, r_day;
    char player_name[MAX_NAME_LENGTH];
    char team_name[MAX_NAME_LENGTH];
    int points, assists, blocks;
    float minutes;

    /* Track current match info */
    int current_year = -1, current_month = -1, current_day = -1;
    int purdue_score = 0, opponent_score = 0;
    char opponent_name[MAX_NAME_LENGTH] = "";
    int wins = 0, losses = 0;
    int found_data = 0;
    int header_written = 0;

    /*
     * Main reading loop using fscanf()
     *
     * fscanf() returns the number of items successfully read
     * We expect 9 items per line:
     *   year, month, day, player_name, team, points, assists, blocks, minutes
     */
    while (fscanf(fp_in, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                  &r_year, &r_month, &r_day,
                  player_name, team_name,
                  &points, &assists, &blocks, &minutes) == 9) {

        /* Validate the date */
        if (!is_valid_date(r_year, r_month, r_day)) {
            fclose(fp_in);
            fclose(fp_out);
            return BAD_DATE;
        }

        /* Validate numeric fields */
        if (points < 0 || assists < 0 || blocks < 0 || minutes <= 0) {
            fclose(fp_in);
            fclose(fp_out);
            return BAD_RECORD;
        }

        /* Check if this is the year we want */
        if (r_year != year) {
            continue;  /* Skip records from other years */
        }

        /*
         * Detect when we move to a new match
         * (different date = different game)
         */
        if (r_year != current_year || r_month != current_month || r_day != current_day) {
            /* If we had a previous match, output its result */
            if (current_year != -1 && current_year == year) {
                /* Write header on first match */
                if (!header_written) {
                    fprintf(fp_out, "%d\n", year);
                    header_written = 1;
                }

                fprintf(fp_out, "%02d-%02d:Purdue(%d)-%s(%d)\n",
                        current_month, current_day,
                        purdue_score, opponent_name, opponent_score);

                /* Track win/loss */
                if (purdue_score > opponent_score) {
                    wins++;
                } else {
                    losses++;
                }
            }

            /* Reset for new match */
            current_year = r_year;
            current_month = r_month;
            current_day = r_day;
            purdue_score = 0;
            opponent_score = 0;
            memset(opponent_name, 0, sizeof(opponent_name));
        }

        found_data = 1;

        /*
         * Accumulate scores
         * strcmp() returns 0 when strings are equal
         */
        if (strcmp(team_name, "Purdue") == 0) {
            purdue_score += points;
        } else {
            opponent_score += points;
            if (opponent_name[0] == '\0') {
                strncpy(opponent_name, team_name, MAX_NAME_LENGTH - 1);
            }
        }
    }

    /* Don't forget to output the last match! */
    if (current_year == year && found_data) {
        if (!header_written) {
            fprintf(fp_out, "%d\n", year);
            header_written = 1;
        }

        fprintf(fp_out, "%02d-%02d:Purdue(%d)-%s(%d)\n",
                current_month, current_day,
                purdue_score, opponent_name, opponent_score);

        if (purdue_score > opponent_score) {
            wins++;
        } else {
            losses++;
        }
    }

    /* Write final record */
    if (found_data) {
        fprintf(fp_out, "Record: %dW-%dL\n", wins, losses);
    }

    /*
     * STEP 4: Close files
     * ALWAYS close files when done! This:
     * - Flushes any buffered data to disk
     * - Releases system resources
     * - Prevents data corruption
     */
    fclose(fp_in);
    fclose(fp_out);

    return found_data ? SUCCESS : NO_DATA_POINTS;
}

/* ============================================================
 * FUNCTION: match_most_valuable_player
 * ============================================================
 *
 * LEARNING POINTS:
 * - Filtering data by multiple criteria (year, month, day)
 * - Calculating derived values (combined score)
 * - Finding maximum value while reading
 */
double match_most_valuable_player(char *in_file, int year, int month, int day) {
    FILE *fp = NULL;

    /* Validate date parameters first */
    if (!is_valid_date(year, month, day)) {
        return (double)BAD_DATE;
    }

    fp = fopen(in_file, "r");
    if (fp == NULL) {
        return (double)FILE_READ_ERR;
    }

    int r_year, r_month, r_day;
    char player_name[MAX_NAME_LENGTH];
    char team_name[MAX_NAME_LENGTH];
    int points, assists, blocks;
    float minutes;

    double max_combined_score = -1.0;
    int found_match = 0;

    while (fscanf(fp, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                  &r_year, &r_month, &r_day,
                  player_name, team_name,
                  &points, &assists, &blocks, &minutes) == 9) {

        if (!is_valid_date(r_year, r_month, r_day)) {
            fclose(fp);
            return (double)BAD_DATE;
        }

        if (points < 0 || assists < 0 || blocks < 0 || minutes <= 0) {
            fclose(fp);
            return (double)BAD_RECORD;
        }

        /* Check if this is the match we're looking for */
        if (r_year == year && r_month == month && r_day == day) {
            found_match = 1;

            /*
             * Calculate combined score using the formula:
             * Combined = points + 1.5*assists + 2*blocks + 0.2*minutes
             */
            double combined = (double)points +
                              1.5 * (double)assists +
                              2.0 * (double)blocks +
                              0.2 * (double)minutes;

            if (combined > max_combined_score) {
                max_combined_score = combined;
            }
        }
    }

    fclose(fp);

    if (!found_match) {
        return (double)NO_DATA_POINTS;
    }

    return max_combined_score;
}

/* ============================================================
 * FUNCTION: average_points_player
 * ============================================================
 *
 * LEARNING POINTS:
 * - String comparison with strcmp()
 * - Accumulating totals and counts
 * - Calculating averages
 */
double average_points_player(char *in_file, char *player_name) {
    FILE *fp = NULL;

    fp = fopen(in_file, "r");
    if (fp == NULL) {
        return (double)FILE_READ_ERR;
    }

    int r_year, r_month, r_day;
    char r_player_name[MAX_NAME_LENGTH];
    char team_name[MAX_NAME_LENGTH];
    int points, assists, blocks;
    float minutes;

    int total_points = 0;
    int match_count = 0;

    while (fscanf(fp, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                  &r_year, &r_month, &r_day,
                  r_player_name, team_name,
                  &points, &assists, &blocks, &minutes) == 9) {

        if (!is_valid_date(r_year, r_month, r_day)) {
            fclose(fp);
            return (double)BAD_DATE;
        }

        if (points < 0 || assists < 0 || blocks < 0 || minutes <= 0) {
            fclose(fp);
            return (double)BAD_RECORD;
        }

        /*
         * strcmp() returns 0 when strings match
         * We need to match the exact player name
         */
        if (strcmp(r_player_name, player_name) == 0) {
            total_points += points;
            match_count++;
        }
    }

    fclose(fp);

    if (match_count == 0) {
        return (double)NO_DATA_POINTS;
    }

    /* Calculate and return average */
    return (double)total_points / (double)match_count;
}

/* ============================================================
 * FUNCTION: purdue_best_winning_match_score
 * ============================================================
 *
 * LEARNING POINTS:
 * - Tracking per-match totals (need to detect match boundaries)
 * - Finding maximum based on calculated difference
 * - Handling ties (highest Purdue score wins)
 */
int purdue_best_winning_match_score(char *in_file, int year, int month) {
    FILE *fp = NULL;

    /* Validate parameters */
    if (year <= 0 || month < 1 || month > 12) {
        return BAD_DATE;
    }

    fp = fopen(in_file, "r");
    if (fp == NULL) {
        return FILE_READ_ERR;
    }

    int r_year, r_month, r_day;
    char player_name[MAX_NAME_LENGTH];
    char team_name[MAX_NAME_LENGTH];
    int points, assists, blocks;
    float minutes;

    /* Current match tracking */
    int current_year = -1, current_month = -1, current_day = -1;
    int purdue_score = 0, opponent_score = 0;

    /* Best winning match tracking */
    int best_difference = -1;
    int best_purdue_score = -1;
    int found_win = 0;

    while (fscanf(fp, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                  &r_year, &r_month, &r_day,
                  player_name, team_name,
                  &points, &assists, &blocks, &minutes) == 9) {

        if (!is_valid_date(r_year, r_month, r_day)) {
            fclose(fp);
            return BAD_DATE;
        }

        if (points < 0 || assists < 0 || blocks < 0 || minutes <= 0) {
            fclose(fp);
            return BAD_RECORD;
        }

        /* Detect new match */
        if (r_year != current_year || r_month != current_month || r_day != current_day) {
            /* Process previous match if it's in our target month/year */
            if (current_year == year && current_month == month) {
                int diff = purdue_score - opponent_score;
                /* Check if Purdue won and if this is the best win */
                if (diff > 0) {
                    if (diff > best_difference ||
                        (diff == best_difference && purdue_score > best_purdue_score)) {
                        best_difference = diff;
                        best_purdue_score = purdue_score;
                        found_win = 1;
                    }
                }
            }

            /* Reset for new match */
            current_year = r_year;
            current_month = r_month;
            current_day = r_day;
            purdue_score = 0;
            opponent_score = 0;
        }

        /* Accumulate scores */
        if (strcmp(team_name, "Purdue") == 0) {
            purdue_score += points;
        } else {
            opponent_score += points;
        }
    }

    /* Process last match */
    if (current_year == year && current_month == month) {
        int diff = purdue_score - opponent_score;
        if (diff > 0) {
            if (diff > best_difference ||
                (diff == best_difference && purdue_score > best_purdue_score)) {
                best_difference = diff;
                best_purdue_score = purdue_score;
                found_win = 1;
            }
        }
    }

    fclose(fp);

    if (!found_win) {
        return NO_DATA_POINTS;
    }

    return best_purdue_score;
}

/* ============================================================
 * FUNCTION: purdue_best_month
 * ============================================================
 *
 * LEARNING POINTS:
 * - Using arrays to track per-month statistics
 * - Calculating rates/percentages
 * - Finding maximum across categories
 */
int purdue_best_month(char *in_file) {
    FILE *fp = NULL;

    fp = fopen(in_file, "r");
    if (fp == NULL) {
        return FILE_READ_ERR;
    }

    /* Arrays to track wins and total games per month (index 0-11 for months 1-12) */
    int wins[12] = {0};
    int total_games[12] = {0};

    int r_year, r_month, r_day;
    char player_name[MAX_NAME_LENGTH];
    char team_name[MAX_NAME_LENGTH];
    int points, assists, blocks;
    float minutes;

    /* Current match tracking */
    int current_year = -1, current_month = -1, current_day = -1;
    int purdue_score = 0, opponent_score = 0;

    while (fscanf(fp, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                  &r_year, &r_month, &r_day,
                  player_name, team_name,
                  &points, &assists, &blocks, &minutes) == 9) {

        if (!is_valid_date(r_year, r_month, r_day)) {
            fclose(fp);
            return BAD_DATE;
        }

        if (points < 0 || assists < 0 || blocks < 0 || minutes <= 0) {
            fclose(fp);
            return BAD_RECORD;
        }

        /* Detect new match */
        if (r_year != current_year || r_month != current_month || r_day != current_day) {
            /* Process previous match */
            if (current_month >= 1 && current_month <= 12) {
                total_games[current_month - 1]++;
                if (purdue_score > opponent_score) {
                    wins[current_month - 1]++;
                }
            }

            current_year = r_year;
            current_month = r_month;
            current_day = r_day;
            purdue_score = 0;
            opponent_score = 0;
        }

        if (strcmp(team_name, "Purdue") == 0) {
            purdue_score += points;
        } else {
            opponent_score += points;
        }
    }

    /* Process last match */
    if (current_month >= 1 && current_month <= 12) {
        total_games[current_month - 1]++;
        if (purdue_score > opponent_score) {
            wins[current_month - 1]++;
        }
    }

    fclose(fp);

    /* Find best month */
    int best_month = -1;
    double best_rate = -1.0;

    for (int i = 0; i < 12; i++) {
        if (total_games[i] > 0) {
            double rate = (double)wins[i] / (double)total_games[i];
            if (rate > best_rate) {
                best_rate = rate;
                best_month = i + 1;  /* Convert back to 1-based month */
            }
        }
    }

    if (best_month == -1 || best_rate == 0.0) {
        return NO_DATA_POINTS;
    }

    return best_month;
}

/* ============================================================
 * FUNCTION: generate_player_report
 * ============================================================
 *
 * LEARNING POINTS:
 * - Combining reading and writing in one function
 * - Tracking multiple statistics
 * - Formatted output with precision specifiers (%.2f)
 */
int generate_player_report(char *in_file, char *player_name, char *out_file) {
    FILE *fp_in = NULL;
    FILE *fp_out = NULL;

    fp_in = fopen(in_file, "r");
    if (fp_in == NULL) {
        return FILE_READ_ERR;
    }

    int r_year, r_month, r_day;
    char r_player_name[MAX_NAME_LENGTH];
    char team_name[MAX_NAME_LENGTH];
    int points, assists, blocks;
    float minutes;

    /* Statistics accumulators */
    int total_points = 0;
    int total_assists = 0;
    int total_blocks = 0;
    float total_minutes = 0.0;
    int games_played = 0;
    int games_won = 0;

    /* We need to track match boundaries to count wins */
    int current_year = -1, current_month = -1, current_day = -1;
    int purdue_score = 0, opponent_score = 0;
    int player_in_this_match = 0;

    /* First pass: collect all data */
    while (fscanf(fp_in, "%d-%d-%d|%63[^,],%63[^#]#%d,%d,%d,%f",
                  &r_year, &r_month, &r_day,
                  r_player_name, team_name,
                  &points, &assists, &blocks, &minutes) == 9) {

        if (!is_valid_date(r_year, r_month, r_day)) {
            fclose(fp_in);
            return BAD_DATE;
        }

        if (points < 0 || assists < 0 || blocks < 0 || minutes <= 0) {
            fclose(fp_in);
            return BAD_RECORD;
        }

        /* Detect new match */
        if (r_year != current_year || r_month != current_month || r_day != current_day) {
            /* Process previous match - count win if player was in it */
            if (player_in_this_match && purdue_score > opponent_score) {
                games_won++;
            }

            current_year = r_year;
            current_month = r_month;
            current_day = r_day;
            purdue_score = 0;
            opponent_score = 0;
            player_in_this_match = 0;
        }

        /* Accumulate scores for win/loss determination */
        if (strcmp(team_name, "Purdue") == 0) {
            purdue_score += points;
        } else {
            opponent_score += points;
        }

        /* Check if this is our player (must be Purdue) */
        if (strcmp(r_player_name, player_name) == 0 && strcmp(team_name, "Purdue") == 0) {
            total_points += points;
            total_assists += assists;
            total_blocks += blocks;
            total_minutes += minutes;
            games_played++;
            player_in_this_match = 1;
        }
    }

    /* Process last match */
    if (player_in_this_match && purdue_score > opponent_score) {
        games_won++;
    }

    fclose(fp_in);

    if (games_played == 0) {
        return NO_DATA_POINTS;
    }

    /* Now write the report */
    fp_out = fopen(out_file, "w");
    if (fp_out == NULL) {
        return FILE_WRITE_ERR;
    }

    /*
     * Write formatted report
     * %.2f means: floating point with exactly 2 decimal places
     */
    fprintf(fp_out, "Player: %s\n", player_name);
    fprintf(fp_out, "Games: %d\n", games_played);
    fprintf(fp_out, "Games Won: %d\n", games_won);
    fprintf(fp_out, "Points per Game: %.2f\n", (double)total_points / games_played);
    fprintf(fp_out, "Assists per Game: %.2f\n", (double)total_assists / games_played);
    fprintf(fp_out, "Blocks per Game: %.2f\n", (double)total_blocks / games_played);
    fprintf(fp_out, "Average Minutes: %.2f\n", total_minutes / games_played);

    fclose(fp_out);

    return SUCCESS;
}
