#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void writeOutput(double* matrix, int m, int n, char* fileName){
 int i,j;
  FILE* filePtr;
  filePtr = fopen(fileName, "w");

  for(i = 0; i < m; i++){
    for(j = 0; j < n; j++){
      fprintf(filePtr, "%f ", matrix[i*n + j]);
    }
    fprintf(filePtr, "\n");
  }

  fclose(filePtr);
}

void printMatrices(double *matrix, int m, int n){
  for(int i=0;i < m; i++){
    for(int j =0; j < n;j++){
      printf("%f ",matrix[i*n+j]);
    }
    printf("%s","\n");
  }
}

int mmult(double *c, 
	  double *a, int aRows, int aCols, 
	  double *b, int bRows, int bCols) {
  int i, j, k;
  for (i = 0; i < aRows; i++) {
    for (j = 0; j < bCols; j++) {
      c[i*bCols + j] = 0;
    }
    for (k = 0; k < aCols; k++) {
      for (j = 0; j < bCols; j++) {
	c[i*bCols + j] += a[i*aCols + k] * b[k*bCols + j];
      }
    }
  }
  return 0;
}

int naive_mmult(double *c, 
	  double *a, int aRows, int aCols, 
	  double *b, int bRows, int bCols){
int i, j, k;
for (i = 0; i < aRows; i++)
  for (j = 0; j < bCols; j++) {
    c[i*bCols + j] = 0;
    for (k = 0; k < bRows; k++)
      c[i*bCols + j] += a[i*bRows + k] * b[k*bCols + j];
  }
}

double* gen_matrix(int n, int m,FILE *fp) {
  int i,j,fileScan;
  char buffer[11 * sizeof(double)];
  double* matrix = malloc(sizeof(double) * m * n);
  for (i=0; i < m; i++){
    for(j=0; j < n; j++){ 
      fileScan = fscanf(fp, "%s", buffer);
      matrix[i*n + j] = atof(buffer);
   }
  }
  return matrix;
}

double* gen_matrix2(int n, int m) { //original gen_matrix method. cant recall if c allows for overloading so changing method name

  int i, j;

  double *a = malloc(sizeof(double) * n * m);

  for (i = 0; i < n; i++) {

    for (j = 0; j < m; j++) {

      a[i*m + j] = (double)rand()/RAND_MAX;

    }

  }

  return a;

}

void compare_matrices(double* a, double* b, int nRows, int nCols) {
	int n = nRows * nCols;
	int i, j, k;
	for (k = 0; k < n; ++k) {
		if (fabs(a[k]-b[k])/fabs(a[k]) > 1e-12) {
			i = k/nCols;
			j = k%nCols;
			printf("a[%d][%d] == %.12g\nb[%d][%d] == %.12g\ndelta == %.12g\nrelerr == %.12g\n",
				i, j, a[k], i, j, b[k], fabs(a[k]-b[k]), fabs(a[k]-b[k])/fabs(a[k]));
			return;
		}
	}
	printf("Matrices are the same\n");
}

