#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <omp.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#define VERTICES 4

unsigned int time_seed() {
    struct timeval t;
    struct timezone tzdummy;

    gettimeofday(&t, &tzdummy);
    return (unsigned int)(t.tv_usec);
}

int graph[VERTICES][VERTICES] = {{0,1,0,1},
                                    {1,0,1,0},
                                    {0,1,0,1},
                                    {1,0,1,0}
                                    };
int flag = 1;

struct Independent_sets{
    int independent_sets[1024][VERTICES+1];
    int size;
}independent_sets;

struct TempSolutionsSet{
    int tempSolutionsSet[VERTICES+1];
    int size;
    int k;
    int vertex;
};

struct Maximal_Independent_sets{
    int max_independent_sets[1024][VERTICES+1];
    int size;
}maximalindependentsets;

void insert(struct TempSolutionsSet temp){
    int size = independent_sets.size;
    independent_sets.independent_sets[size][0] = 0;
    for(int i = 0; i < temp.size; i++){
        independent_sets.independent_sets[size][i+1] = temp.tempSolutionsSet[i];
        independent_sets.independent_sets[size][0]++;
    }
    independent_sets.size = size + 1;
}

void findAllIndepadentSets(int currV, int setSize, struct TempSolutionsSet tempset){
    independent_sets.size = 0;
    int i, j;
    for(i = currV; i < setSize; i++){
        flag = 1;
        #pragma omp parallel for default(private) shared(flag, i)
        {
            for(j = 0; j < tempset.size; j++){
                if(tempset.tempSolutionsSet[j] == i){
                    flag = 0;
                }
            }
        }
        if(flag){
            tempset.tempSolutionsSet[tempset.size] = i;
            tempset.size += 1;
            findAllIndepadentSets(i+1, setSize, tempset);
            tempset.size -= 1;
        }
    }
    insert(tempset);
}

void MaximalIndependentsets(){
    int maxCount = 0;
    maximalindependentsets.size = 0;
    #pragma omp parallel for default(private) shared(maxCount)
    {
        int local_max_count = 0;
        for(int i = 0; i < independent_sets.size; i++){
            int localCount = independent_sets.independent_sets[i][0];
            if(localCount > local_max_count){
                local_max_count = localCount;
            }
        }
        if(maxCount < local_max_count)
            maxCount = local_max_count;
    }

    if(maxCount == 0){
        printf("No maximal independent sets\n");
        return ;
    }
    
    #pragma omp parallel for default(private) shared(maxindependentsets)
    {
        for(int i = 0; i < independent_sets.size; i++){
            int localCount = independent_sets.independent_sets[i][0];
            if(localCount == maxCount){
                int size = maximalindependentsets.size;
                maximalindependentsets.max_independent_sets[size][0] = 0;
                for(int k = 0; k < maxCount; k++){
                    maximalindependentsets.max_independent_sets[size][k+1] = independent_sets.independent_sets[i][k+1];
                    maximalindependentsets.max_independent_sets[size][0] += 1;
                }
                maximalindependentsets.size = size + 1;
            }
        }
    }
    printf("Maximal independent sets: \n");
    for(int i = 0; i < maximalindependentsets.size-1; i++){
        printf("{ ");
        for(int k = 1; k <= maxCount; k++)
            printf("%d ", maximalindependentsets.max_independent_sets[i][k]);
        printf("}\n");
    }

}

int main(){
    srand(time_seed());
    int i, j;
    /*for(i = 0; i < VERTICES; i++){
        for(j = i+1; j < VERTICES; j++){
            if(i == j){
                graph[i][j] = 0;
                continue;
            }
            int r = rand()%100;
            graph[i][j] = r;
            graph[j][i] = r;
        }
    }*/
    printf("=======GRAPH========\n");
    for(i = 0; i < VERTICES; i++){
        for(j = 0; j < VERTICES; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    struct TempSolutionsSet tempset;
    tempset.size = 0;   
    findAllIndepadentSets(0, VERTICES, tempset);
    MaximalIndependentsets();
}