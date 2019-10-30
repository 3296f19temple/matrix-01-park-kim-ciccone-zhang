#include<stdio.h>
#include<stdlib.h> 
#include<math.h>
#include"mmult.c"

double* gen_matrix2(int n, int m);
int mmult(double *c, double *a, int aRows, int aCols, double *b, int bRows, int bCols);
void compare_matrix(double *a, double *b, int nRows, int nCols);
void writeOutput(double* matrix, int m, int n, char* fileName);

int main(int argc, char* argv[]){
  int nrows, ncols, maxDim;
  double *aa; // Matrix A Square
  double *bb; // Matrix B Square
  double *cc1; //A x B Matrix result

  if(argc == 2){//randomly generate other size values for nonsquare matrices
    //nrows = (int)rand() % maxDim;
    nrows = atoi(argv[1]);
    ncols = nrows;

    aa = gen_matrix2(nrows, ncols);
    bb = gen_matrix2(nrows, ncols);
    cc1 = malloc(sizeof(double) * nrows * nrows);

    naive_mmult(cc1, aa, nrows, ncols, bb, nrows, ncols);

    //compare_matrices(cc1, cc1, nrows, nrows);
    writeOutput(cc1, nrows, ncols, "outputSq.txt");
    
  }
  

}
