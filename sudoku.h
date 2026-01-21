#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>
#include <stdio.h>


bool eliminate_row(unsigned int sudoku[9][9], int row_index);
bool eliminate_col(unsigned int sudoku[9][9], int col_index);
bool eliminate_box(unsigned int sudoku[9][9], int row_index, int col_index);
bool needs_solving(unsigned int sudoku[9][9]);
bool is_valid(unsigned int sudoku[9][9]);
bool solve(unsigned int sudoku[9][9]);
bool load(unsigned int sudoku[9][9]);
void print(unsigned int sudoku[9][9]);


#endif //SUDOKU_H
