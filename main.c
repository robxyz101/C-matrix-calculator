#include "matrix.h"

/* Prompts for a yes/no answer. Returns 1 only for 'Y' or 'y'. */
static int confirm(const char *prompt) {
    char c;

    printf("%s [Y/N] ", prompt);
    if (scanf(" %c", &c) != 1) return 0;
    M_ClearInput();
    return c == 'Y' || c == 'y';
}

int main(void) {
    int run = 1;
    int operation;
    int nMat = 0;
    int choice = 0;
    int secChoice = 0;

    TMatrix *mat = NULL;
    TMatrix res;

    printf(""
" ___________.__                 _____             __           .__         \n"
" \\__    ___/|  |__    ____     /     \\  _____   _/  |_ _______ |__|___  ___\n"
"   |    |   |  |  \\ _/ __ \\   /  \\ /  \\ \\__  \\  \\   __\\\\_  __ \\|  |\\  \\/  /\n"
"   |    |   |   Y  \\  ___/  /    Y    \\ / __ \\_ |  |   |  | \\/|  | >    < \n"
"   |____|   |___|  / \\___  > \\____|__  /(____  / |__|   |__|   |__|__/\\_ \\\n"
"                 \\/      \\/          \\/      \\/                          \\/\n"
"\n");

    while (run) {
        printf("\n");
        printf("+++ [1] matrix input \n");
        printf("+++ [2] delete all the matrices \n");
        printf("+++ [3] print all the matrices currently inserted \n");
        printf("+++ [4] sum two matrices \n");
        printf("+++ [5] multiply a matrix by a constant \n");
        printf("+++ [6] multiply two matrices\n");
        printf("+++ [7] transpose a matrix \n");
        printf("+++ [8] invert a matrix \n");
        printf("+++ [9] Gauss elimination (and simultaneous equations solver)\n");
        printf("+++ [10] matrix determinant \n");
        printf("+++ [11] matrix rank \n");
        printf("+++ [12] subtract two matrices \n");
        printf("+++ [other] stop the program\n");

        printf("\nInsert option: ");
        if (scanf("%d", &operation) != 1) {
            M_ClearInput();
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (operation) {
            case 1: {
                int rows, cols;

                printf("--- MATRIX %d ---\n", nMat + 1);
                printf("Insert the number of rows: ");
                if (scanf("%d", &rows) != 1 || rows <= 0) {
                    printf("Insert a valid number of rows\n");
                    M_ClearInput();
                    break;
                }
                printf("Insert the number of columns: ");
                if (scanf("%d", &cols) != 1 || cols <= 0) {
                    printf("Insert a valid number of columns\n");
                    M_ClearInput();
                    break;
                }

                TMatrix *tmp = (TMatrix *)realloc(mat, (nMat + 1) * sizeof(TMatrix));
                if (!tmp) {
                    printf("Memory allocation failed.\n");
                    break;
                }
                mat = tmp;

                M_Create(&mat[nMat], rows, cols);
                if (!M_Insert(&mat[nMat])) {
                    M_Destroy(&mat[nMat]);
                    printf("\nInput ended. Exiting.\n");
                    run = 0;
                    break;
                }
                M_Print(&mat[nMat]);
                nMat++;
                printf("\nReturn to main menu\n");
                break;
            }

            case 2:
                if (nMat == 0) {
                    printf("No matrices have been inserted yet.\n");
                } else if (confirm("Delete all the matrices?")) {
                    for (int i = 0; i < nMat; i++)
                        M_Destroy(&mat[i]);
                    free(mat);
                    mat = NULL;
                    nMat = 0;
                    printf("\nAll matrices have been deleted!\n");
                }
                printf("\nReturn to main menu\n");
                break;

            case 3:
                if (nMat == 0)
                    printf("No matrices have been inserted yet.\n");
                else {
                    for (int i = 0; i < nMat; i++) {
                        printf("\n--- MATRIX %d ---\n", i + 1);
                        M_Print(&mat[i]);
                    }
                }
                printf("\nReturn to main menu\n");
                break;

            case 4:
                if (nMat < 2) { printf("At least two matrices required.\n"); break; }
                printf("Matrix A: ");
                if (scanf("%d", &choice) != 1) { M_ClearInput(); break; }
                printf("Matrix B: ");
                if (scanf("%d", &secChoice) != 1) { M_ClearInput(); break; }

                if (choice < 1 || choice > nMat || secChoice < 1 || secChoice > nMat) {
                    printf("Invalid number.\n"); break;
                }
                if (mat[choice-1].rows != mat[secChoice-1].rows || mat[choice-1].cols != mat[secChoice-1].cols) {
                    printf("Incompatible dimensions for sum.\n"); break;
                }
                M_MatSum(&mat[choice-1], &mat[secChoice-1], &res);
                printf("--- A + B ---\n");
                M_Print(&res);
                M_Destroy(&res);
                break;

            case 5: {
                double k;
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number: ");
                if (scanf("%d", &choice) != 1) { M_ClearInput(); break; }
                if (choice < 1 || choice > nMat) { printf("Invalid number.\n"); break; }
                printf("Constant: ");
                if (scanf("%lf", &k) != 1) { M_ClearInput(); break; }
                M_ConstProd(&mat[choice-1], k);
                M_Print(&mat[choice-1]);
                break;
            }

            case 6:
                if (nMat < 2) { printf("At least two matrices required.\n"); break; }
                printf("Matrix A: ");
                if (scanf("%d", &choice) != 1) { M_ClearInput(); break; }
                printf("Matrix B: ");
                if (scanf("%d", &secChoice) != 1) { M_ClearInput(); break; }

                if (choice < 1 || choice > nMat || secChoice < 1 || secChoice > nMat) {
                    printf("Invalid number.\n"); break;
                }
                if (mat[choice-1].cols != mat[secChoice-1].rows) {
                    printf("Incompatible dimensions for multiplication.\n"); break;
                }
                M_MatMult(&mat[choice-1], &mat[secChoice-1], &res);
                printf("--- A x B ---\n");
                M_Print(&res);
                M_Destroy(&res);
                break;

            case 7:
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number: ");

                if (scanf("%d", &choice) != 1) {
                    M_ClearInput();
                    break;
                }
                if (choice < 1 || choice > nMat) {
                    printf("Invalid number.\n");
                    break;
                }

                M_Transpose(&mat[choice-1], &res);
                M_Destroy(&mat[choice-1]);
                mat[choice-1] = res;

                printf("--- Matrix %d is now Transposed ---\n", choice);
                M_Print(&mat[choice-1]);
                break;

            case 8: {
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number to invert: ");

                if (scanf("%d", &choice) != 1) {
                    M_ClearInput();
                    break;
                }
                if (choice < 1 || choice > nMat) {
                    printf("Invalid number.\n");
                    break;
                }

                if (M_Inverse(&mat[choice-1], &res)) {
                    M_Destroy(&mat[choice-1]);
                    mat[choice-1] = res;

                    printf("--- Matrix %d is now Inverted ---\n", choice);
                    M_Print(&mat[choice-1]);
                }
                break;
            }

            case 9: {
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number: ");

                if (scanf("%d", &choice) != 1) { M_ClearInput(); break; }
                if (choice < 1 || choice > nMat) { printf("Invalid number.\n"); break; }

                TMatrix tmp;
                M_Copy(&mat[choice-1], &tmp);
                M_GaussElimination(&tmp, NULL);
                M_Print(&tmp);
                M_Destroy(&tmp);

                if (confirm("Solve the system?")) {
                    double *sol = (double *)malloc(mat[choice-1].rows * sizeof(double));
                    if (!sol) { printf("Allocation failed.\n"); break; }
                    if (M_SolveSystem(&mat[choice-1], sol) == 1) {
                        printf("Solution:\n");
                        for (int i = 0; i < mat[choice-1].rows; i++)
                            printf("x%d = %g\n", i+1, sol[i]);
                    }
                    free(sol);
                }
                break;
            }

            case 10: {
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number: ");

                if (scanf("%d", &choice) != 1) { M_ClearInput(); break; }
                if (choice < 1 || choice > nMat) { printf("Invalid number.\n"); break; }
                if (mat[choice-1].rows != mat[choice-1].cols) {
                    printf("Determinant defined only for square matrices.\n"); break;
                }

                TMatrix tmp;
                M_Copy(&mat[choice-1], &tmp);
                int swaps = 0;
                M_GaussElimination(&tmp, &swaps);
                double det = M_Determinant(&tmp, swaps);
                if (fabs(det) < 1e-12) det = 0.0;
                printf("Determinant = %.6g\n", det);
                M_Destroy(&tmp);
                break;
            }

            case 11: {
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number to calculate rank: ");

                if (scanf("%d", &choice) != 1) {
                    M_ClearInput();
                    break;
                }
                if (choice < 1 || choice > nMat) {
                    printf("Invalid number.\n");
                    break;
                }

                printf("Rank = %d\n", M_Rank(&mat[choice-1]));
                break;
            }

            case 12:
                if (nMat < 2) { printf("At least two matrices required.\n"); break; }
                printf("Matrix A: ");
                if (scanf("%d", &choice) != 1) { M_ClearInput(); break; }
                printf("Matrix B: ");
                if (scanf("%d", &secChoice) != 1) { M_ClearInput(); break; }

                if (choice < 1 || choice > nMat || secChoice < 1 || secChoice > nMat) {
                    printf("Invalid number.\n"); break;
                }
                if (mat[choice-1].rows != mat[secChoice-1].rows || mat[choice-1].cols != mat[secChoice-1].cols) {
                    printf("Incompatible dimensions for subtraction.\n"); break;
                }
                M_MatSub(&mat[choice-1], &mat[secChoice-1], &res);
                printf("--- A - B ---\n");
                M_Print(&res);
                M_Destroy(&res);
                break;

            default:
                run = 0;
                printf("\nShutting down the program\n");
        }
    }

    if (mat) {
        for (int i = 0; i < nMat; i++)
            M_Destroy(&mat[i]);
        free(mat);
    }
    return 0;
}
