#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct SMatrix {
    double *items;
    int rows;
    int cols;
} TMatrix;


TMatrix *M_Create(TMatrix *mat, int rows, int cols);
void M_Destroy(TMatrix *mat);
void M_Insert(TMatrix *mat);
void M_Print(const TMatrix *mat);

void M_MatSum(const TMatrix *A, const TMatrix *B, TMatrix *C);
void M_MatSub(const TMatrix *A, const TMatrix *B, TMatrix *C);
void M_ConstProd(TMatrix *mat, double k);
void M_MatMult(const TMatrix *A, const TMatrix *B, TMatrix *C);
void M_Transpose(const TMatrix *mat, TMatrix *T);
int M_Inverse(const TMatrix *A, TMatrix *Inv);
void M_GaussElimination(TMatrix *mat, int *swaps);
double M_Determinant(const TMatrix *mat, int row_swaps);
int M_Rank(const TMatrix *mat);
int M_SolveSystem(const TMatrix *mat, double *solution);

void M_SwapRows(TMatrix *mat, int rowS, int rowD);
void M_Copy(const TMatrix *src, TMatrix *dst);

#endif