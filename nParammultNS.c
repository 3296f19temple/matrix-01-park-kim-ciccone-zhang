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
  
  //printf("running nParammultNS with values %d and %d", atoi(argv[1]), atoi(argv[2]));

  if(argc == 3){//randomly generate other size values for nonsquare matrices
    //maxDim = atoi(argv[1]);
    //printf("nParammultNS inside running");
    ncols = atoi(argv[2]);
    nrows = atoi(argv[1]);
    //ncols = nrows;

    aa = gen_matrix2(nrows, ncols);
    bb = gen_matrix2(ncols, nrows);
    cc1 = malloc(sizeof(double) * ncols * ncols);

    mmult(cc1, aa, nrows, ncols, bb, ncols, nrows);

    //compare_matrices(cc1, cc1, nrows, nrows);
    writeOutput(cc1, nrows, nrows, "outputNSq.txt");
    //printf("done\n");
  }
}                                   
