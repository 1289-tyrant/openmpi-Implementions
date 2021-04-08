#include <stdio.h>
#include <limits.h>
#include <omp.h>

#define V 5

int minKey(int key[], int mstSet[])  {  
    int min = INT_MAX, min_index;  
    #pragma omp parallel for defalut(private) shared(min, min_index)
    {
        int temp_min = INT_MAX, temp_min_index;
        for (int v = 0; v < V; v++)  
            if (mstSet[v] == 0 && key[v] < temp_min){
                temp_min = key[v];
                temp_min_index = v;
            }  
        
        if(temp_min < min){
            min = temp_min;
            min_index = temp_min_index;
        }
                
    }  
  
    return min_index;  
}  

void printMST(int parent[], int graph[V][V])  
{  
    printf("Edge \tweigth\n");
    for (int i = 1; i < V; i++)  
        printf("%d - %d\t%d\n", parent[i], i, graph[i][parent[i]]);  
}

void primMST(int graph[V][V])  {  
    int parent[V];    
    int key[V];    
    int mstSet[V];  
    for (int i = 0; i < V; i++)  
        key[i] = INT_MAX, mstSet[i] = 0;  

    key[0] = 0;  
    parent[0] = -1;   
  
    for (int count = 0; count < V - 1; count++) {  
        int u = minKey(key, mstSet);  
  
        mstSet[u] = 1;  
  
        #pragma omp parallel for default(private)
        {
            for (int v = 0; v < V; v++)  
            if (graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v])  
                parent[v] = u, key[v] = graph[u][v];
        }  
    }  
  
    printMST(parent, graph);  
}  

int main(){
    int graph[V][V] = { { 0, 2, 0, 6, 0 },  
                        { 2, 0, 3, 8, 5 },  
                        { 0, 3, 0, 0, 7 },  
                        { 6, 8, 0, 0, 9 },  
                        { 0, 5, 7, 9, 0 } };  
  
    primMST(graph);  
}