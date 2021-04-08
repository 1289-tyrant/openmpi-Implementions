#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BASE_BITS 8
#define BASE (1 << BASE_BITS)
#define MASK (BASE-1)
#define DIGITS(v, shift) (((v) >> shift) & MASK)

void radix_sort(int n, int data[n]) {
    int * buffer = malloc(n*sizeof(int));
    int total_digits = sizeof(int)*8;
 
    //Each thread use local_bucket to move data
    int i;
    for(int shift = 0; shift < total_digits; shift+=BASE_BITS) {
        int bucket[BASE] = {0};
 
        int local_bucket[BASE] = {0};
        #pragma omp parallel firstprivate(local_bucket)
        {
            #pragma omp for schedule(static) nowait
            for(i = 0; i < n; i++){
                local_bucket[DIGITS(data[i], shift)]++;
            }
            #pragma omp critical
            for(i = 0; i < BASE; i++) {
                bucket[i] += local_bucket[i];
            }
            #pragma omp barrier
            #pragma omp single
            for (i = 1; i < BASE; i++) {
                bucket[i] += bucket[i - 1];
            }
            int nthreads = omp_get_num_threads();
            int tid = omp_get_thread_num();
            for(int cur_t = nthreads - 1; cur_t >= 0; cur_t--) {
                if(cur_t == tid) {
                    for(i = 0; i < BASE; i++) {
                        bucket[i] -= local_bucket[i];
                        local_bucket[i] = bucket[i];
                    }
                } else { 
                    #pragma omp barrier
                }
 
            }
            #pragma omp for schedule(static)
            for(i = 0; i < n; i++) { 
                buffer[local_bucket[DIGITS(data[i], shift)]++] = data[i];
            }
        }
        int* tmp = data;
        data = buffer;
        buffer = tmp;
    }
 
   free(buffer);

    for(int i = 0; i < n; i++){
        printf("%d ", data[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("usage: ./proc <NELEMENTS>\n");
    }
    int n = atoi(argv[1]);
    int data[n];
    for(int i = 0; i < n; i++)
        data[i] = rand()%n;
    radix_sort(n, data);
}