#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int n;

int partition(int arr[], int low, int high){
	int i, j, temp, key;
	key = arr[low];
	i= low + 1;
	j= high;
	while(1){
		while(i < high && key >= arr[i])
    		i++;
		
		while(j > low && key < arr[j])
    		j--;
		
		if(i < j){
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
		else{
			temp = arr[low];
			arr[low] = arr[j];
			arr[j]= temp;
			return j;
		}
	}
}

void quicksort(int arr[], int low, int high){
	int j;
	if(low < high){
		j = partition(arr, low, high);
		#pragma omp parallel sections
        {
			#pragma omp section
	        {
        		quicksort(arr, 0, j - 1);
			
			}
			#pragma omp section
			{
        		quicksort(arr, j + 1, n-1);
			}
		}
	}
}

int main(int argc, char *argv[]){
	if(argc != 2){
        printf("usage: ./proc <NELEMENTS>\n");
        return 0;
    }
    n = atoi(argv[1]);
	int arr[n];
    for(int i = 0; i < n; i++){
		arr[i] = rand()%n;
	}
    quicksort(arr, 0, n - 1);
}