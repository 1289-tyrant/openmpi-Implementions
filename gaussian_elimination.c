#include <stdio.h>
#include <omp.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>

#define MAXN 2000

int N = 10;

volatile float A[MAXN][MAXN], B[MAXN], X[MAXN];

unsigned int time_seed() {
    struct timeval t;
    struct timezone tzdummy;

    gettimeofday(&t, &tzdummy);
    return (unsigned int)(t.tv_usec);
}

void initialize_inputs() {
    int row, col;

    printf("\nInitializing...\n");
    for (col = 0; col < N; col++) {
        for (row = 0; row < N; row++) {
        A[row][col] = (float)rand() / 32768.0;
        }
        B[col] = (float)rand() / 32768.0;
        X[col] = 0.0;
    }
}

void print_inputs() {
    int row, col;
    if (N < 10) {
        printf("\nA =\n\t");
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
            printf("%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t");
            }
        }
            printf("\nB = [");
        for (col = 0; col < N; col++) {
            printf("%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
        }
    }
}

void print_X() {
    int row;
    if (N < 100) {
        printf("\nX = [");
        for (row = 0; row < N; row++) {
            printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
        }
    }
}

void swap_col(int col){
    int temp = B[col];
    B[col] = B[0];
    B[0] = temp;
    for(int i = 0; i < N; i++){
        temp = A[i][col];
        A[i][col] = A[0][col];
        A[0][col] = temp;
    }
}

void gauss() {
    int norm, row, col;  
    float multiplier;

    for (norm = 0; norm < N - 1; norm++) {

        int i, j;
        int max_col = INT_MIN, max_loc = 0;
        
        #pragma omp parallel for default(private)
        {
            int col;
            int maxi = INT_MIN, loc = 0;
            for(col = 0; col < N; col++){
                if(A[norm][col] > maxi){
                    maxi = A[norm][col];
                    loc = col;
                }
            }
            if(max_col < maxi){
                max_col = maxi;
                max_loc = loc;
            }
        }

        if(max_col != 0)
            swap_col(max_col);
        #pragma omp parallel for default(private)
        {
            int row, col;
            for(row = norm+1; row < N; row++){
                multiplier = A[row][norm] / A[norm][norm];
                for(col = norm; col < N; col++){
                    A[row][col] -= A[norm][col]*multiplier;
                }
                B[row] -= B[norm] * multiplier;
            }
        }
    }

    /* Back substitution */
    for (row = N - 1; row >= 0; row--) {
        X[row] = B[row];
        for (col = N-1; col > row; col--) {
        X[row] -= A[row][col] * X[col];
        }
        X[row] /= A[row][row];
    }
}

int main(){
    srand(time_seed());
    initialize_inputs();
    print_inputs();
    gauss();
    print_X();
}