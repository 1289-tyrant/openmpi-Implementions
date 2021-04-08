#include <stdio.h>
#include <limits.h>
#include <omp.h>

#define V 9

int minDistance(int dist[], int sptSet[]) 
{ 
    int min = INT_MAX, min_index; 
    #pragma omp parallel for default(private) shared(min, min_index)
    {
        int temp_min = INT_MAX, temp_min_index;
        for (int v = 0; v < V; v++) 
            if (sptSet[v] == 0 && dist[v] <= temp_min) 
                temp_min = dist[v], temp_min_index = v; 

        if(min > temp_min){
            min_index = temp_min_index;
            min = temp_min;
        }
    }
  
    return min_index; 
} 
  
void printSolution(int dist[]) 
{ 
    printf("Vertex \t\t Distance from Source\n"); 
    for (int i = 0; i < V; i++) 
        printf("%d \t\t %d\n", i, dist[i]); 
} 
  
void dijkstra(int graph[V][V], int src) 
{ 
    int dist[V];  
  
    int sptSet[V];
    for (int i = 0; i < V; i++) 
        dist[i] = INT_MAX, sptSet[i] = 0; 
  
    dist[src] = 0; 
  
    for (int count = 0; count < V - 1; count++) { 
        int u = minDistance(dist, sptSet); 
  
        sptSet[u] = 1; 
  
        #pragma omp parallel for default(private)
        {
            for (int v = 0; v < V; v++) 
                if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) 
                    dist[v] = dist[u] + graph[u][v];
        } 
    } 
  
    printSolution(dist); 
} 

int main(){
    int graph[V][V] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 }, 
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 }, 
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 }, 
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 }, 
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 }, 
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 }, 
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 }, 
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 }, 
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } }; 
  
    dijkstra(graph, 0); 
}