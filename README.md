# C Matrix Calculator

A command-line application written in C for matrix operations and solving linear systems. The program uses dynamic memory allocation to handle matrices of arbitrary dimensions and provides an interactive, menu-driven interface.

## Features

- **Basic Operations**: Matrix addition, scalar multiplication, and matrix multiplication.
- **Advanced Operations**: Transpose, Determinant, Rank calculation, and Matrix Inversion (via Gauss-Jordan elimination).
- **Linear Systems**: Gaussian elimination (row echelon form) and a built-in solver for systems of linear equations using augmented matrices.
- **Memory Management**: Matrices are stored in a dynamically reallocated array. All memory is safely tracked and freed either upon user request or graceful program exit to prevent memory leaks.

## Project Structure

- `matrix.h`: Header file containing the `TMatrix` struct definition and function prototypes.
- `matrix.c`: Core implementation of the mathematical algorithms and memory management routines.
- `TheMatrix.c`: CLI loop, user input validation, and terminal output formatting.

## Build Instructions

You will need a standard C compiler such as `gcc` or `clang`.

**Note for Linux/Unix users:** Because the program relies on the standard math library (`<math.h>`) for floating-point absolute value calculations (`fabs`), you must explicitly link the math library using the `-lm` flag.

```bash
# Compile the source files
gcc main.c matrix.c -o matrix_calc -lm

# Run the executable
./matrix_calc
