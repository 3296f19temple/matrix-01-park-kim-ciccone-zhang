#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int mmult(double *c,double *a, int aRows, int aCols,double *b, int bRows, int bCols) {
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

int mmult_std(double *c,
              double *a, int aRows, int aCols,
              double *b, int bRows, int bCols) {
    
    //A standard version of the matrix multiplication algorithm to use for control data
    
    int i, j, k, N;
    N = aRows;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c[i*bCols + j] = 0;
            for (k = 0; k < N; k++) {
                c[i*aCols + j] += a[i*aCols + k] * b[k*bCols + j];
            }
        }
    }
    return 0;
}

double* gen_matrix_n(int n, int m) {
  int i, j;
  double *a = malloc(sizeof(double) * n * m);
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      a[i*m + j] = (double)rand()/RAND_MAX;
    }
  }
  return a;
}

double* gen_matrix(int n, int m, FILE *fp) {
    int i, j;
    double x;
    double *a = malloc(sizeof(double) * n * m);
    char buf[4096];
    
    //reset file pointer to start of file
    rewind(fp);
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            //scan an integer from the file and place it into the matrix
            if(!(fscanf(fp, "%lf", &x))) {
                fprintf(stderr, "Error reading file while generating matrix");
                exit(EXIT_FAILURE);
            }
            //printf("\t%f", x);
            a[i*m + j] = x;
        }
        //puts("");
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

