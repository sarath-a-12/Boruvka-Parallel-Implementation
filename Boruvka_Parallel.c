#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void readGraph(int* indexArray, int* edgeArray){
    indexArray[0] = 1;
    edgeArray[2] = 2;
}

int main(){
    //Read the input graph in CSR format
    int noOfVertices, noOfEdges;
    scanf("%d %d", &noOfVertices, &noOfEdges);
    int *indexArray = (int *) malloc(sizeof(int) * noOfVertices);
    int *edgeArray = (int *) malloc(sizeof(int) * noOfEdges * 2); //to store the weights along with the edges
    readGraph(indexArray, edgeArray);
    printf("%d %d", *(indexArray), *(edgeArray + 2));
}
