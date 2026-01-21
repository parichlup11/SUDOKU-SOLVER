====================SIMPLE SUDOKU SOLVER=====================
                           


A terminal-based Sudoku solver written in C. I created this project
to practice logic implementation and bitwise operations.

WHAT IT DOES:
- Logic Solver: Tries to solve the puzzle using basic elimination rules (rows, 
  cols, squares), just like a human would.
- Efficiency: Uses bits (bitmasks) to represent possible numbers in each cell, 
  which makes checking for conflicts very fast.

HOW TO USE IT:
1. Compile the project:
   $ make

2. Solve a puzzle from a text file:
   $ ./sudoku --solve --print < input_example.txt
