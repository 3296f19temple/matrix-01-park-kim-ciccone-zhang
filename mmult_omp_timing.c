#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>

int mmult(double *c, 
	  double *a, int aRows, int aCols, 
	  double *b, int bRows, int bCols);

int mmult_omp(double *c,
		   double *a, int aRows, int aCols,
		   double *b, int bRows, int bCols);

int mmult_std(double *c,
          double *a, int aRows, int aCols,
          double *b, int bRows, int bCols);

double* gen_matrix_n(int n, int m);
double* gen_matrix(int n, int m, FILE *fp);

void compare_matrices(double* a, double* b, int nRows, int nCols);

double deltaTime(struct timespec* start, struct timespec* end) {
  double delta = (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec)/1e9;
  return delta;
}

int main(int argc, char* argv[]) {
  struct timespec start;
  struct timespec end;
  struct timespec res;
  double *a, *b, *c1, *c2, *c3;
  int n;
  double times[3];
  if (argc > 1) {
      
      if (argc > 2) {
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
          
          n = 0;
          //get rows
          while(fgets(buf, 4096, mat1)) {
              //printf("ROW: %s\n", buf);
              n++;
          }
          
          //get columns
          int k = 0;
          int ncols1 = 0;
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
          int nrows2 = 0;
          while(fgets(buf, 4096, mat2)) {
              //printf("ROW: %s\n", buf);
              nrows2++;
          }
          
          //get B columns
          int j = 0;
          int ncols2 = 0;
          while(buf[j] != '\n') {
              //printf("buf[%d]:\t%c\n", k, buf[j]);
              
              if(isspace(buf[j])) {
                  ncols2++;
              }
              j++;
          }
          ncols2++;
          
          //check validity of matrix multiplication
          if(ncols1 != nrows2) {
              fprintf(stderr, "Matrix size mismatch.");
              exit(EXIT_FAILURE);
          }
          
          a = gen_matrix(n, ncols1, mat1);
          b = gen_matrix(nrows2, ncols2, mat2);
      }
      else {
          n = atoi(argv[1]);
          a = gen_matrix_n(n, n);
          b = gen_matrix_n(n, n);
      }
      
    c1 = malloc(sizeof(double) * n * n);
    c2 = malloc(sizeof(double) * n * n);
    c3 = malloc(sizeof(double) * n * n);
    //Standard
    clock_gettime(CLOCK_REALTIME, &start);
    mmult_std(c3, a, n, n, b, n, n);
    clock_gettime(CLOCK_REALTIME, &end);
    times[0] = deltaTime(&start, &end);
    printf("Order: Matrix size, time for standard, time for SIMD, time for OMP.\n");
    printf("%d %f", n, times[0]);
    //SIMD
    clock_gettime(CLOCK_REALTIME, &start);
    mmult(c1, a, n, n, b, n, n);
    clock_gettime(CLOCK_REALTIME, &end);
    times[1] = deltaTime(&start, &end);
    printf(" %f", n, times[1]);
    //OMP
    clock_gettime(CLOCK_REALTIME, &start);
    mmult_omp(c2, a, n, n, b, n, n);
    clock_gettime(CLOCK_REALTIME, &end);
    times[2] = deltaTime(&start, &end);
    printf(" %f", times[2]);
    printf("\n");
    compare_matrices(c1, c2, n, n);
    compare_matrices(c3, c1, n, n);
  } else {
    fprintf(stderr, "Usage %s <n>\n", argv[0]);
  }
}

