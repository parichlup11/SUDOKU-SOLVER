#include "sudoku.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Silence levels */
static const int SILENCE_NO_REPORT = 1;
static const int SILENCE_NO_RESULT = 2;

/**
 * Helper to extract the unique number from a cell bitmask.
 * Returns the number (1-9) or 0 if not unique/set.
 */
static int get_number(unsigned int sudoku[9][9], int row, int col)
{
    int value = sudoku[row][col];
    // If all bits are set (511 = 111111111 binary), it's empty
    if (value == 511) {
        return 0;
    }
    
    // Check if exactly one bit is set
    int result = 1;
    while (value != 1) {
        if (value == 0) return 0; 
        result++;
        value /= 2;
    }
    return result;
}

static void raw_print(unsigned int sudoku[9][9])
{
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int n = get_number(sudoku, row, col);
            putchar('0' + n);
        }
    }
    putchar('\n');
}

static void print_binary(int n)
{
    for (int i = 8; i >= 0; --i) {
        putchar((n & (1 << i)) ? '1' : '0');
    }
    putchar('\n');
}

static void usage(const char *program)
{
    printf("Usage: %s OPTIONS\n\n", program);
    printf("Options:\n");
    printf("\t--help\t\tShow this message\n");
    printf("\t--load\t\tLoad sudoku from stdin (default)\n");
    printf("\t--print\t\tPrint sudoku as ASCII grid\n");
    printf("\t--raw\t\tPrint sudoku as raw digit string\n");
    printf("\t--solve\t\tSolve the puzzle (elimination)\n");
    printf("\t--check-valid\tCheck validity\n");
    printf("\t--cell INDEX\tShow binary value of cell (00-88)\n");
    printf("\n");
}

/* --- Demo Helper Functions --- */

static void print_cell_value(unsigned int sudoku[9][9], const char *optarg, int silent)
{
    char *endptr = NULL;
    int pos = strtol(optarg, &endptr, 10);
    
    int row = pos / 10;
    int col = pos % 10;

    if (row < 0 || row > 8 || col < 0 || col > 8) {
        fprintf(stderr, "Invalid cell index %s\n", optarg);
        return;
    }

    if (silent < SILENCE_NO_REPORT)
        printf("\nBINARY VALUE OF CELL [%i,%i]:\n", row, col);

    print_binary(sudoku[row][col]);
}

static void init_rand(const char *optarg)
{
    char *endptr = NULL;
    int seed = strtol(optarg, &endptr, 10);
    srand(seed);
}

/* --- Main --- */

int main(int argc, char **argv)
{
    srand(time(NULL));

    unsigned int sudoku[9][9];
    bool valid_load = false;
    int silent = 0;

    // First pass: Check for help, silence, or explicit load
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "--load") == 0 || strcmp(argv[i], "--LOAD") == 0) {
            valid_load = true;
        } else if (strcmp(argv[i], "--silent") == 0) {
            ++silent;
        }
    }

    // Implicit load if no arguments prevent it
    if (!valid_load) {
        if (silent < SILENCE_NO_REPORT)
            puts("Loading Sudoku...");
            
        if (!load(sudoku)) {
            if (silent < SILENCE_NO_RESULT)
                fprintf(stderr, "Failed to load Sudoku.\n");
            return EXIT_FAILURE;
        }
    }

    // Second pass: Process commands
    for (int argi = 1; argi < argc; ++argi) {
        const char *option = argv[argi];
        const char *optarg = argv[argi + 1];

        if (strcmp(option, "--raw") == 0) {
            if (silent < SILENCE_NO_REPORT) puts("\nRAW OUTPUT:");
            raw_print(sudoku);
        } 
        else if (strcmp(option, "--print") == 0) {
            if (silent < SILENCE_NO_REPORT) puts("\nGRID OUTPUT:");
            print(sudoku);
        } 
        else if (strcmp(option, "--cell") == 0) {
            if (argi + 1 >= argc) return EXIT_FAILURE;
            ++argi;
            print_cell_value(sudoku, optarg, silent);
        } 
        else if (strcmp(option, "--solve") == 0) {
            if (silent < SILENCE_NO_REPORT) puts("\nSOLVING...");
            bool done = solve(sudoku);
            if (silent < SILENCE_NO_RESULT)
                puts(done ? "SOLVED" : "FAILED TO SOLVE");
        } 
        else if (strcmp(option, "--check-valid") == 0) {
             if (silent < SILENCE_NO_RESULT)
                puts(is_valid(sudoku) ? "VALID" : "INVALID");
        } 
        else if (strcmp(option, "--seed") == 0) {
            if (argi + 1 >= argc) return EXIT_FAILURE;
            ++argi;
            init_rand(optarg);
        }
        else if (strcmp(option, "--eliminate-row") == 0) {
             if (argi + 1 >= argc) return EXIT_FAILURE;
             ++argi;
             eliminate_row(sudoku, atoi(optarg));
        }
        else if (strcmp(option, "--eliminate-col") == 0) {
             if (argi + 1 >= argc) return EXIT_FAILURE;
             ++argi;
             eliminate_col(sudoku, atoi(optarg));
        }
        else if (strcmp(option, "--load") == 0 || strcmp(option, "--silent") == 0) {
            // Handled in first pass
        } 
        else {
            // Ignore unknown options
        }
    }

    return EXIT_SUCCESS;
}