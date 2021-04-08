#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int shellSort(int arr[], int n) 
{ 
	for (int gap = n/2; gap > 0; gap /= 2) 
	{ 
        #pragma omp parallel for default(private)
        for (int i = gap; i < n; i += 1){ 
			int temp = arr[i]; 

			int j;			 
			#pragma omp parallel for default(private) shared(arr)
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) 
				arr[j] = arr[j - gap]; 
			
			arr[j] = temp; 
		} 
	} 
	return 0; 
} 

void printArray(int arr[], int n) 
{ 
	for (int i=0; i<n; i++) 
		printf("%d ", arr[i]); 
    printf("\n");
} 

int main() 
{ 
	int n = 100;
    int arr[n], i; 

    srand(10);
    
    for(int i = 0; i < n; i++)
        arr[i] = rand()%n;

	printf("Array before sorting: \n"); 
	printArray(arr, n); 

	shellSort(arr, n); 

	printf("\nArray after sorting: \n"); 
	printArray(arr, n); 

	return 0; 
} 
