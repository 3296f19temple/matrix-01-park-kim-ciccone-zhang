#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <ctype.h>
#define min(x, y) ((x)<(y)?(x):(y))

double* gen_matrix(int n, int m, FILE *fp);
int mmult(double *c, double *a, int aRows, int aCols, double *b, int bRows, int bCols);
void compare_matrix(double *a, double *b, int nRows, int nCols);
void read_matrix(double *mat, int rows, int cols);

int main(int argc, char* argv[]) {
    int nrows1, nrows2, ncols1, ncols2;
    double *aa;    /* the A matrix */
    double *bb;    /* the B matrix */
    double *cc1;    /* A x B computed using the omp-mpi code you write */
    double *cc2;    /* A x B computed using the conventional algorithm */
    int myid, numprocs;
    double starttime, endtime;
    int numsent, sender, anstype;
    int master = 0;
    FILE *out;
    MPI_Status status;
    /* insert other global variables here */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if(argc > 1) {
        FILE *mat1;
        FILE *mat2;
        char buf[4096];

        //open matrix files
        if(!(mat1 = fopen(argv[1], "r"))) {
            fprintf(stderr, "Matrix 1 file error.");
            exit(EXIT_FAILURE);
        }
        if(!(mat2 = fopen(argv[2], "r"))) {
            fprintf(stderr, "Matrix 2 file error.");
            exit(EXIT_FAILURE);
        }

        nrows1 = 0;
        //get rows
        while(fgets(buf, 4096, mat1)) {
            //printf("ROW: %s\n", buf);
            nrows1++;
        }

        //get columns
        int k = 0;
        ncols1 = 0;
        while(buf[k] != '\n') {
            //printf("buf[%d]:\t%c\n", k, buf[k]);

            //if we reach a whitespace, a number logically preceded it, therefore ++ the column count
            if(isspace(buf[k])) {
                ncols1++;
            }
            k++;
        }
        //no whitespace after last number, so needs to be added manually
        ncols1++;

        //now get rows for matrix B
        nrows2 = 0;
        while(fgets(buf, 4096, mat2)) {
            //printf("ROW: %s\n", buf);
            nrows2++;
        }

        //get B columns
        int j = 0;
        ncols2 = 0;
        while(buf[j] != '\n') {
            //printf("buf[%d]:\t%c\n", k, buf[j]);

            if(isspace(buf[j])) {
                ncols2++;
            }
            j++;
        }
        ncols2++;

        /** Test
        printf("# of rows1:\t%d\n", nrows1);
        printf("# of rows2:\t%d\n", nrows2);
        printf("# of cols1:\t%d\n", ncols1);
        printf("# of cols2:\t%d\n", ncols2);
        */

        //check validity of matrix multiplication
        if(ncols1 != nrows2) {
            fprintf(stderr, "Matrix size mismatch.");
            exit(EXIT_FAILURE);
        }

        //create matrices
        aa = gen_matrix(nrows1, ncols1, mat1);
        bb = gen_matrix(nrows2, ncols2, mat2);
        cc1 = malloc(sizeof(double) * nrows1 * ncols2);
        cc2 = malloc(sizeof(double) * nrows1 * ncols2);

        if(myid == 0) {
            // Master Code goes here
            starttime = MPI_Wtime();

            /* Insert your master code here to store the product into cc1 */
            numsent = 0;
            MPI_Bcast(bb, nrows2 * ncols2, MPI_DOUBLE, master, MPI_COMM_WORLD);

            //send a row to slaves
            for(int i = 0; i < min(nrows1, numprocs - 1); i++) {
                MPI_Send(&aa[(i) * nrows1], ncols1, MPI_DOUBLE, i + 1, i + 1, MPI_COMM_WORLD);
                numsent++;
            }

            //receive info from slaves
            double *receive = malloc(sizeof(double) * ncols2);

            for(int i = 0; i < nrows1; i++) {
                MPI_Recv(receive, ncols2, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sender = status.MPI_SOURCE;
                anstype = status.MPI_TAG;

                //fill cc1 with the data received
                for(int k = 0; k < ncols2; k++) {
                    cc1[(anstype - 1) * ncols2 + k] = receive[k];
                }

                //if more work for slaves, give work
                if(numsent < nrows1) {
                    MPI_Send(&aa[numsent * nrows1], ncols1, MPI_DOUBLE, sender, numsent + 1, MPI_COMM_WORLD);
                    numsent++;
                }else { //tell slaves no more work
                    MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
                }
            }

            endtime = MPI_Wtime();
            printf("Time:\t%f\n", (endtime - starttime));

            //compute cc2 using standard algorithm
            mmult(cc2, aa, nrows1, ncols1, bb, nrows2, ncols2);

            //compare matrices
            compare_matrices(cc2, cc1, nrows1, ncols2);

            //open output file
            if(!(out = fopen("c.txt", "w+"))) {
                fprintf(stderr, "Error creating output file.");
            }

            //write results to file
            for(int q = 0; q < nrows1; q++) {
                for(int r = 0; r < ncols2; r++) {
                    fprintf(out, "%f ", cc2[q * ncols2 + r]);
                }
                fprintf(out, "%s", "\n");
            }

        }else {
            //slave stuck
            // Slave Code goes here

            MPI_Bcast(bb, nrows2 * ncols2, MPI_DOUBLE, master, MPI_COMM_WORLD);
            if(myid <= nrows1) {
                while(1) {
                    double buffer[ncols1];
                    double ans[ncols2];

                    MPI_Recv(buffer, ncols1, MPI_DOUBLE, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    if(status.MPI_TAG == 0) {
                        break;
                    }
                    anstype = status.MPI_TAG;
                    for(int i = 0; i < ncols2; i++) {
                        double sum = 0;
                        for(int j = 0; j < ncols1; j++) {
                            sum = sum + (buffer[j] * bb[j * ncols2 + i]);
                        }
                        ans[i] = sum;
                    }
                    MPI_Send(ans, ncols2, MPI_DOUBLE, master, anstype, MPI_COMM_WORLD);
                }
            }
        }

        fclose(mat1);
        fclose(mat2);
        fclose(out);
    }else {
        fprintf(stderr, "Usage mmult_mpi_omp <matrix_file1> <matrix_file2>\n");
    }
    MPI_Finalize();
    return 0;
}
