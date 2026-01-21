#include "sudoku.h"

#include <ctype.h>

#define MAX_ROWS 13
#define MAX_COLUMNS 26

/*
 * Eliminates possibilities in a specific row.
 * If a number is already set (only 1 bit is 1), it removes that number 
 * from possibilities of other cells in the same row.
 */
bool eliminate_row(unsigned int sudoku[9][9], int row_index)
{
    bool changed = false;
    unsigned int value_list[9] = { 0 };

    // First pass: Iterate through the row to find solved cells
    for (int j = 0; j < 9; j++) {
        unsigned int value = sudoku[row_index][j];
        value_list[j] = sudoku[row_index][j];
        
        // Skip if cell is not solved yet (more than 1 bit set) or empty
        if ((value & (value - 1)) == 0 || value == 0) {
            continue;
        }

        // Calculate sum of all solved numbers in this row
        unsigned int sum_of_powers_of_2 = 0;
        for (int k = 0; k < 9; k++) {
            unsigned int cell_value = sudoku[row_index][k];
            // Check if cell has exactly one bit set (is solved)
            if ((cell_value & (cell_value - 1)) == 0) {
                sum_of_powers_of_2 += cell_value;
            }
        }

        // Invert the sum to get a mask of allowed numbers
        unsigned int inverted_sum = ~sum_of_powers_of_2 & 0x1FF;
        
        // Apply mask to current cell
        sudoku[row_index][j] = inverted_sum & value;
        changed = true;
    }

    // Check if any change actually happened compared to original state
    for (int k = 0; k < 9; k++) {
        if (value_list[k] == sudoku[row_index][k]) {
            return false;
        }
    }
    return changed;
}

/*
 * Eliminates possibilities in a specific column.
 * Logic is identical to row elimination, just traversing vertically.
 */
bool eliminate_col(unsigned int sudoku[9][9], int col_index)
{
    bool changed = false;
    unsigned int value_list[9] = { 0 };

    for (int j = 0; j < 9; j++) {
        unsigned int value = sudoku[j][col_index];
        value_list[j] = sudoku[j][col_index];
        
        if ((value & (value - 1)) == 0 || value == 0) {
            continue;
        }

        unsigned int sum_of_powers_of_2 = 0;
        for (int k = 0; k < 9; k++) {
            unsigned int cell_value = sudoku[k][col_index];
            if ((cell_value & (cell_value - 1)) == 0) {
                sum_of_powers_of_2 += cell_value;
            }
        }

        unsigned int inverted_sum = ~sum_of_powers_of_2 & 0x1FF;
        sudoku[j][col_index] = inverted_sum & value;
        changed = true;
    }

    for (int k = 0; k < 9; k++) {
        if (value_list[k] == sudoku[k][col_index]) {
            return false;
        }
    }
    return changed;
}

/*
 * Eliminates possibilities in a 3x3 box.
 * Calculates the top-left corner of the box based on row/col index.
 */
bool eliminate_box(unsigned int sudoku[9][9], int row_index, int col_index)
{
    bool changed = false;
    unsigned int value_list[9] = { 0 };
    
    // Find start of the 3x3 block
    int start_row = 3 * (row_index / 3);
    int start_col = 3 * (col_index / 3);

    for (int i = start_row; i < start_row + 3; i++) {
        for (int j = start_col; j < start_col + 3; j++) {
            unsigned int value = sudoku[i][j];
            value_list[j] = sudoku[i][j];
            
            if ((value & (value - 1)) == 0 || value == 0) {
                continue;
            }

            unsigned int sum_of_powers_of_2 = 0;
            // Sum all solved numbers in the box
            for (int k = start_row; k < start_row + 3; k++) {
                for (int l = start_col; l < start_col + 3; l++) {
                    unsigned int cell_value = sudoku[k][l];
                    if ((cell_value & (cell_value - 1)) == 0) {
                        sum_of_powers_of_2 += cell_value;
                    }
                }
            }
            
            unsigned int inverted_sum = ~sum_of_powers_of_2 & 0x1FF;
            sudoku[i][j] = inverted_sum & value;
            changed = true;
        }
    }
    
    // Check for changes (simplified check logic)
    for (int k = 0; k < 9; k++) {
        for (int i = start_row; i < start_row + 3; i++) {
            for (int j = start_col; j < start_col + 3; j++) {
                if (value_list[k] == sudoku[i][j]) {
                    return false;
                }
            }
        }
    }
    return changed;
}

/*
 * Checks if the sudoku is completely solved.
 * Returns true if there is at least one cell with multiple possibilities (0x1FF).
 */
bool needs_solving(unsigned int sudoku[9][9])
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == 0x1FF) {
                return true;
            }
        }
    }
    return false;
}

// Helper: checks if cell has exactly one bit set (power of 2)
bool is_declared_cell(unsigned int cell)
{
    if ((cell & (cell - 1)) == 0) {
        return true;
    }
    return false;
}

/*
 * Validates the current state of the Sudoku.
 * Checks if there are no duplicate numbers in rows, columns, and boxes.
 * Also checks if no cell is empty (0).
 */
bool is_valid(unsigned int sudoku[9][9])
{
    int rowTotalSum = 0;
    int rowNonZeroDigits = 0;
    int colTotalSum = 0;
    int colNonZeroDigits = 0;
    int remainder = 0;

    // Check rows and columns
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Check for invalid empty cells
            if (sudoku[i][j] == 0 || sudoku[j][i] == 0) {
                return false;
            }
            
            // Sum up values for row check
            if (is_declared_cell(sudoku[i][j])) {
                rowTotalSum += sudoku[i][j];
                rowNonZeroDigits++;
            }
            // Sum up values for col check
            if (is_declared_cell(sudoku[j][i])) {
                colTotalSum += sudoku[j][i];
                colNonZeroDigits++;
            }
        }
        
        // Use bit manipulation to check for duplicates
        // If we sum unique powers of 2, bits shouldn't overlap
        while (rowTotalSum != 0) {
            remainder = rowTotalSum & 1;
            rowTotalSum >>= 1;
            if (remainder == 1) {
                rowNonZeroDigits--;
            }
        }
        while (colTotalSum != 0) {
            remainder = colTotalSum & 1;
            colTotalSum >>= 1;
            if (remainder == 1) {
                colNonZeroDigits--;
            }
        }
        
        if (rowNonZeroDigits != 0 || colNonZeroDigits != 0) {
            return false;
        }
    }

    // Check 3x3 boxes
    int boxTotalSum = 0;
    int boxNonZeroDigits = 0;
    for (int k = 0; k < 9; k += 3) {
        for (int l = 0; l < 9; l += 3) {
            for (int i = k; i < k + 3; i++) {
                for (int j = l; j < l + 3; j++) {
                    if (is_declared_cell(sudoku[i][j])) {
                        boxTotalSum += sudoku[i][j];
                        boxNonZeroDigits++;
                    }
                }
            }
            
            while (boxTotalSum != 0) {
                remainder = boxTotalSum & 1;
                boxTotalSum >>= 1;
                if (remainder == 1) {
                    boxNonZeroDigits--;
                }
            }
            if (boxNonZeroDigits != 0) {
                return false;
            }
        }
    }
    return true;
}

/*
 * Main solver loop.
 * Repeatedly calls elimination functions until no more changes occur.
 */
bool solve(unsigned int sudoku[9][9])
{
    if (!is_valid(sudoku)) {
        return false;
    }
    bool changed = true;
    while (changed) {
        changed = false;
        // Try eliminating rows
        for (int i = 0; i < 9; i++) {
            if (eliminate_row(sudoku, i)) {
                changed = true;
            }
        }
        // Try eliminating columns
        for (int j = 0; j < 9; j++) {
            if (eliminate_col(sudoku, j)) {
                changed = true;
            }
        }
        // Try eliminating boxes
        for (int i = 0; i < 9; i += 3) {
            for (int j = 0; j < 9; j += 3) {
                if (eliminate_box(sudoku, i, j)) {
                    changed = true;
                }
            }
        }
    }
    return is_valid(sudoku);
}

// Converts a number (0-8) to power of 2 (bitmask)
int to_exp(int exp) 
{
    int result = 1;
    for (int i = 0; i < exp - 1; i++) {
        result *= 2;
    }
    return result;
}

/* --- Loading Functions --- */

int load_simple(char input, unsigned int sudoku[9][9])
{
    if (input == '0') {
        sudoku[0][0] = 0x1FF; // 9 bits set (all possibilities)
    } else {
        sudoku[0][0] = to_exp(input - '0');
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            input = getchar();
            if (input == '0') {
                sudoku[i][j] = 0x1FF;
            } else if (isdigit(input)) {
                sudoku[i][j] = to_exp(input - '0');
            } else {
                fprintf(stderr, "Fail while reading input\n");
                return 0;
            }
        }
    }
    // Check for garbage at end
    input = getchar();
    if (input == EOF || input == '\n') {
        return 1;
    }
    if (input != EOF && input != '\n') {
        fprintf(stderr, "Garbage at the end of sudoku\n");
        return 0;
    }
    return 3;
}

int load_complex(unsigned int sudoku[9][9])
{
    char input;
    int row = 0;
    int col = 0;
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            input = getchar();
            
            // Validation logic for formatting characters
            if (j == MAX_COLUMNS - 2 && input != '\n') {
                fprintf(stderr, "added unnecessary char %c\n", input);
                return 0;
            }
            if (i == MAX_ROWS - 1 && (input == EOF || input == '\n')) {
                return 1;
            }
            if (input == '|' || input == '+' || input == '-' || input == '=' ||
                    input == ' ' || input == '\n') {
                continue;
            }
            
            if (row >= 9 || col >= 9) {
                fprintf(stderr, "Too many input numbers\n");
                return 0;
            }

            // Map characters to bitmasks
            if (input == '.') {
                sudoku[row][col] = 0x1FF;
            } else if (input == '!') {
                sudoku[row][col] = 0;
            } else if (isdigit(input)) {
                sudoku[row][col] = to_exp(input - '0');
            } else {
                fprintf(stderr, "Invalid char %c\n", input);
                return 0;
            }
            
            col++;
            if (col == 9) {
                col = 0;
                row++;
            }
        }
    }
    return 3;
}

/*
 * Main load function. 
 * Detects format (simple string vs complex grid) based on first char.
 */
bool load(unsigned int sudoku[9][9])
{
    char input;
    while ((input = getchar()) != EOF) {
        if (isdigit(input)) {
            int simple_result = load_simple(input, sudoku);
            if (simple_result == 0) return false;
            if (simple_result == 1) return true;

        } else if (input == '+') {
            int complex_result = load_complex(sudoku);
            if (complex_result == 0) return false;
            if (complex_result == 1) return true;
            
        } else if (input == '\n') {
            continue;
        } else {
            fprintf(stderr, "Invalid type of input\n");
            return false;
        }
    }
    return true;
}

// Helper to count bits (power of 2 to integer index)
int bin_to_print(int bin)
{
    int count = 0;
    while (bin != 0) {
        count++;
        bin >>= 1;
    }
    return count;
}

/*
 * Prints the Sudoku grid in ASCII format.
 * '!' for error, '.' for empty, numbers for solved cells.
 */
void print(unsigned int sudoku[9][9])
{
    printf("+=======+=======+=======+\n");
    for (int i = 0; i < 9; i++) {
        printf("| ");
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == 0x1FF) {
                printf(". ");
            } else if (sudoku[i][j] == 0) {
                printf("! ");
            } else if ((sudoku[i][j] & (sudoku[i][j] - 1)) != 0) {
                printf(". ");
            } else {
                printf("%d ", bin_to_print(sudoku[i][j]));
            }
            
            if (j == 8) {
                printf("|");
            } else if ((j + 1) % 3 == 0) {
                printf("| ");
            }
        }
        printf("\n");
        if ((i + 1) % 3 == 0) {
            printf("+=======+=======+=======+\n");
        }
    }
}