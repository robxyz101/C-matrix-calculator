#include "matrix.h"

static const double EPS = 1e-12;

/* Counts the rows of mat with a nonzero entry among the first ncols columns. */
static int count_nonzero_rows(const TMatrix *mat, int ncols) {
    int count = 0;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < ncols; j++) {
            if (fabs(mat->items[i * mat->cols + j]) > EPS) {
                count++;
                break;
            }
        }
    }
    return count;
}

TMatrix *M_Create(TMatrix *mat, int rows, int cols) {
    mat->rows = rows;
    mat->cols = cols;
    mat->items = (double *)calloc((size_t)rows * cols, sizeof(double));
    if (mat->items == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return mat;
}

void M_Copy(const TMatrix *src, TMatrix *dst) {
    M_Create(dst, src->rows, src->cols);
    memcpy(dst->items, src->items, (size_t)src->rows * src->cols * sizeof(double));
}

void M_ClearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int M_Insert(TMatrix *mat) {
    printf("\nInsert the elements of the matrix:\n");
    for (int i = 0; i < mat->rows; i++)
        for (int j = 0; j < mat->cols; j++) {
            int status;
            printf("Element [%d][%d]: ", i, j);
            while ((status = scanf("%lf", &mat->items[i * mat->cols + j])) != 1) {
                if (status == EOF) return 0;
                printf("Invalid input. Enter a number: ");
                M_ClearInput();
            }
        }
    return 1;
}

void M_Print(const TMatrix *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            double v = mat->items[i * mat->cols + j];
            if (fabs(v) < EPS) v = 0.0;
            printf("%10.4f ", v);
        }
        printf("\n");
    }
}

void M_Destroy(TMatrix *mat) {
    if (mat && mat->items) {
        free(mat->items);
        mat->items = NULL;
        mat->rows = mat->cols = 0;
    }
}

void M_MatSum(const TMatrix *A, const TMatrix *B, TMatrix *C) {
    M_Create(C, A->rows, A->cols);
    for (int i = 0; i < A->rows * A->cols; i++)
        C->items[i] = A->items[i] + B->items[i];
}

void M_MatSub(const TMatrix *A, const TMatrix *B, TMatrix *C) {
    M_Create(C, A->rows, A->cols);
    for (int i = 0; i < A->rows * A->cols; i++)
        C->items[i] = A->items[i] - B->items[i];
}

void M_ConstProd(TMatrix *mat, double k) {
    for (int i = 0; i < mat->rows * mat->cols; i++)
        mat->items[i] *= k;
}

void M_MatMult(const TMatrix *A, const TMatrix *B, TMatrix *C) {
    M_Create(C, A->rows, B->cols);
    for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < B->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < A->cols; k++)
                sum += A->items[i * A->cols + k] * B->items[k * B->cols + j];
            C->items[i * C->cols + j] = sum;
        }
}

void M_Transpose(const TMatrix *mat, TMatrix *T) {
    M_Create(T, mat->cols, mat->rows);
    for (int i = 0; i < mat->rows; i++)
        for (int j = 0; j < mat->cols; j++)
            T->items[j * T->cols + i] = mat->items[i * mat->cols + j];
}

int M_Inverse(const TMatrix *A, TMatrix *Inv) {
    if (A->rows != A->cols) {
        printf("Error: Matrix must be square to be invertible.\n");
        return 0;
    }

    int n = A->rows;
    TMatrix aug;

    /* Gauss-Jordan elimination on the augmented matrix [A | I]. */
    M_Create(&aug, n, 2 * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug.items[i * aug.cols + j] = A->items[i * A->cols + j];
            aug.items[i * aug.cols + (j + n)] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int k = 0; k < n; k++) {
        int pivot = k;
        double maxv = fabs(aug.items[k * aug.cols + k]);
        for (int i = k + 1; i < n; i++) {
            double v = fabs(aug.items[i * aug.cols + k]);
            if (v > maxv) { maxv = v; pivot = i; }
        }

        if (maxv < EPS) {
            printf("Error: Matrix is singular (determinant is 0), cannot be inverted.\n");
            M_Destroy(&aug);
            return 0;
        }

        M_SwapRows(&aug, k, pivot);

        double pivot_val = aug.items[k * aug.cols + k];
        for (int j = k; j < 2 * n; j++)
            aug.items[k * aug.cols + j] /= pivot_val;

        for (int i = 0; i < n; i++) {
            if (i != k) {
                double factor = aug.items[i * aug.cols + k];
                for (int j = k; j < 2 * n; j++)
                    aug.items[i * aug.cols + j] -= factor * aug.items[k * aug.cols + j];
            }
        }
    }

    M_Create(Inv, n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            Inv->items[i * Inv->cols + j] = aug.items[i * aug.cols + (j + n)];

    M_Destroy(&aug);
    return 1;
}

void M_SwapRows(TMatrix *mat, int rowS, int rowD) {
    if (rowS == rowD) return;
    for (int j = 0; j < mat->cols; j++) {
        double t = mat->items[rowS * mat->cols + j];
        mat->items[rowS * mat->cols + j] = mat->items[rowD * mat->cols + j];
        mat->items[rowD * mat->cols + j] = t;
    }
}

void M_GaussElimination(TMatrix *mat, int *swaps) {
    int rows = mat->rows, cols = mat->cols;
    int r = 0;
    if (swaps) *swaps = 0;

    for (int c = 0; c < cols && r < rows; c++) {
        int pivot = r;
        double maxv = fabs(mat->items[r * cols + c]);
        for (int i = r + 1; i < rows; i++) {
            double v = fabs(mat->items[i * cols + c]);
            if (v > maxv) { maxv = v; pivot = i; }
        }
        if (maxv < EPS) continue;

        if (pivot != r) {
            M_SwapRows(mat, r, pivot);
            if (swaps) (*swaps)++;
        }
        for (int i = r + 1; i < rows; i++) {
            double f = mat->items[i * cols + c] / mat->items[r * cols + c];
            for (int j = c; j < cols; j++)
                mat->items[i * cols + j] -= f * mat->items[r * cols + j];
            mat->items[i * cols + c] = 0.0;
        }
        r++;
    }
}

double M_Determinant(const TMatrix *mat, int row_swaps) {
    if (mat->rows != mat->cols) {
        printf("Error: Determinant is defined only for square matrices.\n");
        return 0.0;
    }

    double det = 1.0;
    for (int i = 0; i < mat->rows; i++)
        det *= mat->items[i * mat->cols + i];
    if (row_swaps % 2) det = -det;
    return det;
}

int M_Rank(const TMatrix *mat) {
    TMatrix tmp;

    M_Copy(mat, &tmp);
    M_GaussElimination(&tmp, NULL);
    int rank = count_nonzero_rows(&tmp, tmp.cols);
    M_Destroy(&tmp);
    return rank;
}

int M_SolveSystem(const TMatrix *mat, double *sol) {
    if (mat->cols != mat->rows + 1) {
        printf("Error: augmented matrix must be n x (n+1).\n");
        return 0;
    }

    int n = mat->rows;
    TMatrix ech;
    int status;

    M_Copy(mat, &ech);
    M_GaussElimination(&ech, NULL);

    /* Rouche-Capelli: compare the rank of the coefficients with that of [A | b]. */
    int rank_coeff = count_nonzero_rows(&ech, n);
    int rank_aug = count_nonzero_rows(&ech, ech.cols);

    if (rank_coeff != rank_aug) {
        printf("No solution (inconsistent system).\n");
        status = 0;
    } else if (rank_coeff < n) {
        printf("Infinite solutions.\n");
        status = -1;
    } else {
        for (int i = n - 1; i >= 0; i--) {
            double sum = 0.0;
            for (int j = i + 1; j < n; j++)
                sum += ech.items[i * ech.cols + j] * sol[j];
            sol[i] = (ech.items[i * ech.cols + n] - sum) / ech.items[i * ech.cols + i];
        }
        status = 1;
    }

    M_Destroy(&ech);
    return status;
}
