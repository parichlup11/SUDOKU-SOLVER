================================================================================
                           SIMPLE SUDOKU SOLVER
================================================================================

A terminal-based Sudoku solver and generator written in C. I created this project
to practice logic implementation and bitwise operations.

WHAT IT DOES:
- Logic Solver: Tries to solve the puzzle using basic elimination rules (rows, 
  cols, squares), just like a human would.
- Backtracking: If the puzzle is too hard for basic logic, it uses recursion 
  to guess numbers and find the solution.
- Generator: Can create new random Sudoku puzzles.
- Efficiency: Uses bits (bitmasks) to represent possible numbers in each cell, 
  which makes checking for conflicts very fast.

HOW TO USE IT:
1. Compile the project:
   $ make

2. Solve a puzzle from a text file:
   $ ./sudoku --solve < puzzle.txt

3. Generate a new puzzle:
   $ ./sudoku --generate --print
