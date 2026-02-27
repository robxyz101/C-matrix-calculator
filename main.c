#include "matrix.h"


int main(void) {
    int run = 1;
    int operation;
    int nMat = 0;
    int choice = 0;
    int secChoice = 0;
    char yn;

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
        printf("+++ [2] matrix delete \n");
        printf("+++ [3] print all the matrices currently inserted \n");
        printf("+++ [4] sum two matrices \n");
        printf("+++ [5] multiply a matrix by a constant \n");
        printf("+++ [6] multiply two matrices\n");
        printf("+++ [7] transpose a matrix \n");
        printf("+++ [8] reverse a matrix \n");
        printf("+++ [9] Gauss elimination (and simultaneous equations solver)\n");
        printf("+++ [10] matrix determinant \n");
        printf("+++ [11] matrix rank \n");
        printf("+++ [other] stop the program\n");

        printf("\nInsert option: ");
        if (scanf("%d", &operation) != 1) {
            while (getchar() != '\n') { } // clear buffer
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
                    while (getchar() != '\n') { }
                    break;
                }
                printf("Insert the number of columns: ");
                if (scanf("%d", &cols) != 1 || cols <= 0) {
                    printf("Insert a valid number of columns\n");
                    while (getchar() != '\n') { }
                    break;
                }

                TMatrix *tmp = (TMatrix *)realloc(mat, (nMat + 1) * sizeof(TMatrix));
                if (!tmp) {
                    printf("Memory allocation failed.\n");
                    break;
                }
                mat = tmp;

                M_Create(&mat[nMat], rows, cols);
                M_Insert(&mat[nMat]);
                M_Print(&mat[nMat]);
                nMat++;
                printf("\nReturn to main menu\n");
                break;
            }

            case 2:
                if (nMat == 0) {
                    printf("No matrices have been inserted yet.\n");
                } else {
                    printf("Delete all the matrices? [Y/N] ");
                    scanf(" %c", &yn);
                    if (yn == 'Y' || yn == 'y') {
                        for (int i = 0; i < nMat; i++)
                            M_Destroy(&mat[i]);
                        free(mat);
                        mat = NULL;
                        nMat = 0;
                        printf("\nAll matrices have been deleted!\n");
                    }
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
                if (scanf("%d", &choice) != 1) { while(getchar() != '\n') { } break; }
                printf("Matrix B: ");
                if (scanf("%d", &secChoice) != 1) { while(getchar() != '\n') { } break; }

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
                if (scanf("%d", &choice) != 1) { while(getchar() != '\n') { } break; }
                if (choice < 1 || choice > nMat) { printf("Invalid number.\n"); break; }
                printf("Constant: ");
                if (scanf("%lf", &k) != 1) { while(getchar() != '\n') { } break; }
                M_ConstProd(&mat[choice-1], k);
                M_Print(&mat[choice-1]);
                break;
            }

            case 6:
                if (nMat < 2) { printf("At least two matrices required.\n"); break; }
                printf("Matrix A: ");
                if (scanf("%d", &choice) != 1) { while(getchar() != '\n') { } break; }
                printf("Matrix B: ");
                if (scanf("%d", &secChoice) != 1) { while(getchar() != '\n') { } break; }

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
                    while(getchar() != '\n') {}
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
                    while(getchar() != '\n') { }
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
                int swaps_local = 0;
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number: ");

                if (scanf("%d", &choice) != 1) { while(getchar() != '\n') { } break; }
                if (choice < 1 || choice > nMat) { printf("Invalid number.\n"); break; }

                TMatrix tmp;
                M_Copy(&mat[choice-1], &tmp);

                M_GaussElimination(&tmp, &swaps_local);
                M_Print(&tmp);

                printf("Solve the system? [Y/N] ");
                scanf(" %c", &yn);
                if (yn == 'Y' || yn == 'y') {
                    double *sol = (double *)malloc(tmp.rows * sizeof(double));
                    if (!sol) { printf("Allocation failed.\n"); M_Destroy(&tmp); break; }
                    int res_val = M_SolveSystem(&tmp, sol);
                    if (res_val == 1) {
                        printf("Solution:\n");
                        for (int i = 0; i < tmp.rows; i++)
                            printf("x%d = %g\n", i+1, sol[i]);
                    }
                    free(sol);
                }
                M_Destroy(&tmp);
                break;
            }

            case 10: {
                if (nMat == 0) { printf("No matrices available.\n"); break; }
                printf("Matrix number: ");

                if (scanf("%d", &choice) != 1) { while(getchar() != '\n') { } break; }
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
                    while(getchar() != '\n') { }
                    break;
                }
                if (choice < 1 || choice > nMat) {
                    printf("Invalid number.\n");
                    break;
                }

                int rank = M_Rank(&mat[choice-1]);
                printf("Rank = %d\n", rank);

                break;
            }

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