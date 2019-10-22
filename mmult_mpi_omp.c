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
void writeToFile(double* matrix, int m, int n, char* fileName);
/** 
    Program to multiply a matrix times a matrix using both
    mpi to distribute the computation among nodes and omp
    to distribute the computation among threads.
*/

int main(int argc, char* argv[])
{
  double *aa;	/* the A matrix */
  double *bb;	/* the B matrix */
  double *cc1;	/* A x B computed using the omp-mpi code you write */
  double *cc2;	/* A x B computed using the conventional algorithm */
  int myid, numprocs;
  double starttime, endtime;
  MPI_Status status;
  int numSent = 0;
  double *buffer;
  int sender;
  int ansType;
  int aRows, bRows, aCols, bCols;
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
    if(aCols != bRows || aRows!= bCols){
      fprintf(stderr, "*******Matrices have different dimensions!*******\n");
      exit(1);
    }     
    
    if (myid == MASTER) {
      // Master Code goes here
      aa = gen_matrix(aRows, aCols, matrixAPtr);
      bb = gen_matrix(bRows, bCols, matrixBPtr);
      cc1 = malloc(sizeof(double) * aRows * aCols); 
      starttime = MPI_Wtime();
      MPI_Bcast(bb, bRows*bCols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      for(int i = 0; i < min(aRows, numprocs-1); i++) {//send rows of A to the slaves for processing
        MPI_Send(&aa[(i)*aRows], aCols, MPI_DOUBLE, i+1, i+1, MPI_COMM_WORLD);
	numSent++;
      }

      buffer = malloc(sizeof(double) * aCols);

      for(int i = 0; i< aRows; i++){ 
        MPI_Recv(buffer, bCols, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        sender = status.MPI_SOURCE;
        ansType = status.MPI_TAG;

        //fill in cc1 with data received from slave
        for(int j=0; j< bCols; j++) {
          cc1[(ansType-1)*bCols + j] = buffer[j];
        }

        if(numSent < aRows) {//if there are more rows to send, send them
          MPI_Send(&aa[numSent*aRows], aCols, MPI_DOUBLE, sender, numSent+1,MPI_COMM_WORLD);
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
    } else {
      // Slave Code goes here
      
      bb = malloc(sizeof(double) * bRows * bCols);
      MPI_Bcast(bb, bRows*bCols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      if(myid <= aRows) {//if there are more proccesses than rows, extra ones immediately exit
        while(1) {
          double input[aCols];
          double output[bCols];
          
	  //listening for work to do
          MPI_Recv(&input, aCols, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          if(status.MPI_TAG == 0){ 
            break;
          }
	  
          ansType = status.MPI_TAG;
          for (int i=0; i < bCols; i++){
	    double sum = 0;
            for(int j=0; j < aCols; j++) {
              sum = sum + (input[j] * bb[j*bCols + i]);
	    }
	    output[i] = sum;
          }
	  //send results to master
          MPI_Send(output, bCols, MPI_DOUBLE, 0, ansType, MPI_COMM_WORLD);
        }
      }
      
    }
    
  } else {
    fprintf(stderr, "Usage matrix_times_vector <size>\n");
  }
  MPI_Finalize();
  return 0;
}

