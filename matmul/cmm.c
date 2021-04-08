#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>

#define MAX 2

int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];

unsigned int time_seed() {
    struct timeval t;
    struct timezone tzdummy;

    gettimeofday(&t, &tzdummy);
    return (unsigned int)(t.tv_usec);
}

void lshift(int r, int s){
    for(int i = 0; i < s; i++){
        int a = matA[r][0];
        for(int j = 1; j < MAX; j++){
            matA[r][j-1] = matA[r][j];
        }
        matA[r][MAX-1] = a;
    }
}

void ushift(int c, int s){
    for(int i = 0; i < s; i++){
        int a = matB[0][c];
        for(int j = 1; j < MAX; j++){
            matB[j-1][c] = matB[j][c];
        }
        matB[MAX-1][c] = a;
    }
}

void mult(){
    #pragma omp parallel for default(private) shared(matA, matB, matC)
    {
        for(int i = 0; i < MAX; i++){
            for(int j = 0; j < MAX; j++)
                matC[i][j] += matA[i][j] * matB[i][j];
        }
    }
}

void shift(){
    #pragma omp parallel default(private) shared(matA, matB, matC) 
    {
        #pragma omp for nowait
        for(int r = 0; r < MAX; r++){
            lshift(r, 1);    
        }

        #pragma omp for
        for(int c = 0; c < MAX; c++){
            ushift(c, 1);
        }
    }
}

int main(){
    srand(time_seed());
    for (int i = 0; i < MAX; i++) { 
        for (int j = 0; j < MAX; j++) { 
            matA[i][j] = rand() % 10; 
            matB[i][j] = rand() % 10; 
        } 
    }  
    printf("\nMatrix A\n");
    for (int i = 0; i < MAX; i++) { 
        for (int j = 0; j < MAX; j++)  
            printf("%d ", matA[i][j]); 
        printf("\n"); 
    } 
  
    printf("\nMatrix B\n");  
    for (int i = 0; i < MAX; i++) { 
        for (int j = 0; j < MAX; j++)  
            printf("%d ", matB[i][j]); 
        printf("\n"); 
    }
    #pragma omp parallel default(private) shared(matA, matB, matC) 
    {
        #pragma omp for nowait
        for(int r = 0; r < MAX; r++){
            lshift(r, r);    
        }

        #pragma omp for
        for(int c = 0; c < MAX; c++){
            ushift(c, c);
        }
    }

    for(int i = 0; i < MAX; i++){
        mult();
        shift();
    }
    printf("\nMultiplication of A and B\n");
    for (int i = 0; i < MAX; i++) { 
        for (int j = 0; j < MAX; j++)  
            printf("%d ", matC[i][j]);         
        printf("\n"); 
    } 
}   