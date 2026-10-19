#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Dense matrix of doubles stored in row-major order. */
typedef struct SMatrix {
    double *items;
    int rows;
    int cols;
} TMatrix;

/*
 * Allocates a zero-initialized rows x cols matrix in *mat.
 * *mat must not own memory already, otherwise it is leaked.
 * Terminates the program if the allocation fails.
 */
TMatrix *M_Create(TMatrix *mat, int rows, int cols);

/* Releases the storage of *mat and resets its dimensions. Safe to call twice. */
void M_Destroy(TMatrix *mat);

/*
 * Reads rows * cols elements from stdin, prompting again on invalid input.
 * Returns 1 on success, 0 if the input stream ends before all elements are read.
 */
int M_Insert(TMatrix *mat);

/* Prints the matrix to stdout. */
void M_Print(const TMatrix *mat);

/* Discards the characters left in stdin up to the end of the current line. */
void M_ClearInput(void);

/* C = A + B. A and B must have the same dimensions. */
void M_MatSum(const TMatrix *A, const TMatrix *B, TMatrix *C);

/* C = A - B. A and B must have the same dimensions. */
void M_MatSub(const TMatrix *A, const TMatrix *B, TMatrix *C);

/* Multiplies every element of *mat by k, in place. */
void M_ConstProd(TMatrix *mat, double k);

/* C = A * B. The columns of A must equal the rows of B. */
void M_MatMult(const TMatrix *A, const TMatrix *B, TMatrix *C);

/* T = transpose of mat. */
void M_Transpose(const TMatrix *mat, TMatrix *T);

/*
 * Computes the inverse of A with Gauss-Jordan elimination into *Inv.
 * Returns 1 on success, 0 if A is not square or is singular; *Inv is
 * left untouched in that case.
 */
int M_Inverse(const TMatrix *A, TMatrix *Inv);

/*
 * Reduces mat to row echelon form in place, using partial pivoting.
 * If swaps is not NULL, it receives the number of row exchanges performed.
 */
void M_GaussElimination(TMatrix *mat, int *swaps);

/*
 * Returns the determinant of a square matrix already reduced by
 * M_GaussElimination, given the number of row exchanges it performed.
 * Prints an error and returns 0 if the matrix is not square.
 */
double M_Determinant(const TMatrix *mat, int row_swaps);

/* Returns the rank of mat. The matrix is not modified. */
int M_Rank(const TMatrix *mat);

/*
 * Solves the linear system described by the n x (n+1) augmented matrix mat,
 * storing the n unknowns in solution. The matrix is not modified.
 * Returns 1 if the solution is unique, 0 if the system has no solution
 * (or the matrix has the wrong shape), -1 if it has infinitely many.
 * solution is written only when 1 is returned.
 */
int M_SolveSystem(const TMatrix *mat, double *solution);

/* Exchanges the rows rowS and rowD of mat. */
void M_SwapRows(TMatrix *mat, int rowS, int rowD);

/* Makes dst an independent copy of src. dst must not own memory already. */
void M_Copy(const TMatrix *src, TMatrix *dst);

#endif
