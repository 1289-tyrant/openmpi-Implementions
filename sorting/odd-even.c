#include <stdio.h>
#include <omp.h>
#include <limits.h>
#include <stdlib.h>  
 

void swap(int *a, int *b){
    int temp;
    if(*a > *b){
        temp = *a;
        *a = *b; 
        *b = temp;
    }
}


void oddEvenSort(int arr[], int N) 
{ 
	for(int n = 0; n < N; n++) {
        int i;
        if(n & 1){
            #pragma omp parallel for private(i) shared(arr)
            for(i = 2; i < N; i+=2)
            if(arr[i-1] > arr[i])
                swap(&arr[i-1],&arr[i]);
        } 
        else {
            #pragma omp parallel for private(i) shared(arr)
            for(i = 1; i < N; i+=2)
            if(arr[i-1] > arr[i])
                swap(&arr[i-1],&arr[i]);
        }
    }

	return; 
} 

void printArray(int arr[], int n) { 
    for (int i=0; i < n; i++) 
        printf("%d ",arr[i]); 
    printf("\n");
} 

int main() { 
	int arr[] = {34, 2, 10, -9, 7, 50, -42, 80, 11, 14}; 
	int n = sizeof(arr)/sizeof(arr[0]); 

	oddEvenSort(arr, n); 
	printArray(arr, n); 

	return (0); 
} 
