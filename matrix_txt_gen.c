//
//  matrix_txt_gen.c
//  
//
//  Class to generate two txt files contatining an N x N matrix given an input of the value of N
//

#include <stdio.h>
#include <stdlib.h>
#include "mmult.c"

double* gen_matrix_n(int n, int m);

int main(int argc, char* argv[]) {
    
    double *a, *b;
    int n, i, j;
    FILE *outA, *outB;
    
    if (argc > 1) {
        
        n = atoi(argv[1]);
        a = gen_matrix_n(n, n);
        b = gen_matrix_n(n, n);
        
        outA = fopen("a.txt", "w+");
        outB = fopen("b.txt", "w+");
        
        if (outA == NULL || outB == NULL) {
            fprintf(stderr, "Unable to create / open text files.\n");
            exit(1);
        }
        
        for (i = 0; i < n; i++) {
            for (j = 0; j < n - 1; j++) {
                fprintf(outA, "%f ", a[i*n + j]);
                fprintf(outB, "%f ", b[i*n + j]);
            }
            fprintf(outA, "%f", a[i*n + j + 1]);
            fprintf(outB, "%f", b[i*n + j + 1]);
            fprintf(outA, "\n");
            fprintf(outB, "\n");
        }
        
        fclose(outA);
        fclose(outB);
        exit(0);
    }
    else {
        fprintf(stderr, "Usage %s <n>\n", argv[0]);
        exit(1);
    }
}
