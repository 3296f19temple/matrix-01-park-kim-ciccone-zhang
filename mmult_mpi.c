
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#define min(x, y) ((x)<(y)?(x):(y))

void writeOutput(double* matrix, int m, int n, char* fileName);
void printMatrices(double *matrix,int m, int n);
double* gen_matrix(int n, int m, FILE* fp);
int mmult(double *c, double *a, int aRows, int aCols, double *b, int bRows, int bCols);
void compare_matrix(double *a, double *b, int nRows, int nCols);

/**
    Program to multiply a matrix times a matrix using both
    mpi to distribute the computation among nodes and omp
    to distribute the computation among threads.
*/

int main(int argc, char* argv[])
{
  double *aa;   /* the A matrix */
  double *bb;   /* the B matrix */
  double *cc1;  /* A x B computed using the omp-mpi code you write */
  double *cc2;  /* A x B computed using the conventional algorithm */
  int myid, numprocs;
  double starttime, endtime;
  MPI_Status status;
  int numSent = 0;
  double *buffer;
  int sender;
  int ansType;
  int toSend;
  double *sendBuff;
  int aRows, bRows, aCols, bCols, nRows, nCols;
  int MASTER = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (argc > 1) {
    FILE* matrixAPtr , *matrixBPtr;
    matrixAPtr = fopen(argv[1],"r");
    matrixBPtr = fopen(argv[2],"r");
    fscanf(matrixAPtr, "rows(%d) cols(%d)", &aRows, &aCols);
    fscanf(matrixBPtr, "rows(%d) cols(%d)", &bRows, &bCols);
    if(aCols != bRows){
      fprintf(stderr, "*******Matrices have different dimensions!*******\n");
      exit(1);
    }
    nRows = aRows; // get number of rows and cols for new matrix
    nCols = bCols;
    toSend = aRows/numprocs;// get number of rows to send to each process
    sendBuff = malloc(sizeof(double) * toSend * aCols);
    double *outPut = (double *) malloc(sizeof(double)* nCols);
    aa = gen_matrix(aRows, aCols, matrixAPtr);
    bb = gen_matrix(bRows, bCols, matrixBPtr);
    if (myid == MASTER) {
      // Master Code goes here
      cc1 = malloc(sizeof(double) * nRows * nCols);
      starttime = MPI_Wtime();
      MPI_Bcast(bb, bRows*bCols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      int i,j,k; //send rows to slaves for processing
      for(i = 0; i < min(aRows, numprocs-1); i++) {
         for(j = 0; j < aCols; j++) {
             sendBuff[j] = aa[(i * aCols) + j];
         }  
         MPI_Send(sendBuff, aCols, MPI_DOUBLE, i+1, i+1, MPI_COMM_WORLD);
         numSent++;
      }


      buffer = malloc(sizeof(double) * aCols);

      for(int i = 0; i< aRows; i++){
        MPI_Recv(outPut, nCols, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        sender = status.MPI_SOURCE;
        ansType = status.MPI_TAG;

        //fill in cc1 with data received from slave
        for(int j=0; j< bCols; j++) {
          cc1[(ansType-1)*bCols + j] = outPut[j];
        }

        if(numSent < aRows) {//if there are more rows to send, send them
          for(j = 0; j < aCols; j++) {
             sendBuff[j] = aa[(numSent * aCols) + j];
          }
          MPI_Send(sendBuff, aCols, MPI_DOUBLE, sender, numSent+1, MPI_COMM_WORLD);
          numSent++;
      }


      buffer = malloc(sizeof(double) * aCols);

      for(int i = 0; i< aRows; i++){
        MPI_Recv(outPut, nCols, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        sender = status.MPI_SOURCE;
        ansType = status.MPI_TAG;

        //fill in cc1 with data received from slave
        for(int j=0; j< bCols; j++) {
          cc1[(ansType-1)*bCols + j] = outPut[j];
        }

        if(numSent < aRows) {//if there are more rows to send, send them
          for(j = 0; j < aCols; j++) {
             sendBuff[j] = aa[(numSent * aCols) + j];
          }
          MPI_Send(sendBuff, aCols, MPI_DOUBLE, sender, numSent+1, MPI_COMM_WORLD);
          numSent++;
        } 
        else {//stop slaves
          MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
        }
      }
      endtime = MPI_Wtime();
      printf("%f\n",(endtime - starttime));

      cc2  = malloc(sizeof(double) * aRows * aRows);
      mmult(cc2, aa, aRows, aCols, bb, bCols, bRows);
      compare_matrices(cc2, cc1, aRows, aRows);
        //TESTING
      /* printMatrices(aa,aRows, bCols);
      printMatrices(bb,bRows,bCols);*/
      printMatrices(cc1, aRows, bCols);
      writeOutput(cc1, aRows, bCols, "output.txt");

      fclose(matrixAPtr);
      fclose(matrixBPtr);

    }
    }
    else {
      // Slave Code goes here
      bb = malloc(sizeof(double) * nRows*nCols);
      MPI_Bcast(bb, bRows*bCols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      if(myid <= aRows) {
        while(1) {
          MPI_Recv(sendBuff, aCols, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          if(status.MPI_TAG == 0){
            break;
          }

          ansType = status.MPI_TAG;
          for (int i=0; i < bCols; i++){
            double sum = 0;
            for(int j=0; j < aCols; j++) {
              sum = sum + (sendBuff[j] * bb[j*bCols + i]);
            }
            outPut[i] = sum;
          }

          MPI_Send(outPut, bCols, MPI_DOUBLE, 0, ansType, MPI_COMM_WORLD);
          if(numSent == aRows){
            break;
          }
        } 
     }
  }
  }
  else {
    fprintf(stderr, "Usage matrix_times_vector <size>\n");
  }
  MPI_Finalize();
  return 0;
}              
