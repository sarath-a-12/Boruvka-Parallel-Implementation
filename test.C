#include<map>
#include<algorithm>
#include<unordered_map>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<omp.h>
#include<climits>
#include "./headers/HSetOPT.h"
using namespace std;
/*
   Graph in CSR format
   */

class Edge{
    public:
        int in;
        int out;
        int edgeWeight;

        Edge(int u, int v, int weight){
            in = u;
            out = v;
            edgeWeight = weight;
        }
};

vector<int> indexArray(8);
vector<int> edgeWeightArray(48);
HSet disSet;
vector<Edge> minimumSpanningTree;



/*
   function declarations 
   */

void readGraph();
void getEdges(char);
void printLoop(vector<int>&);
void printEdges(vector<Edge>&);
char getChar(int);
int getMinEdge(int);
bool checkIfSameParent(vector<int>&);

int main(){
    readGraph();
    int i;
    disSet.Allocate(7);
    cout<<"The edges of MST are:"<<endl;

    int counter = 0;
    int numOfComponents = 7;
    while(numOfComponents > 1){
        vector<int> min_edges(7, INT_MAX);
#pragma omp parallel for
        for(int i = 0; i < 7; i++){
            int repVertex = disSet.Findcompress(i);
            int minEdgeIndex = getMinEdge(i);
            if (minEdgeIndex == -1) 
                continue;
            int minEdgeWeight = edgeWeightArray[2* minEdgeIndex + 1];
            int minWeightOutNeighbor = edgeWeightArray [2* minEdgeIndex];
            cout<<getChar(i)<<' '<<getChar(minWeightOutNeighbor)<<' '<<minEdgeWeight<<endl;
            //use critical to update the min_edges
#pragma omp critical
            {
                if(minEdgeWeight < min_edges[repVertex]) {
                    min_edges[repVertex] = minEdgeIndex;
                }
            }
        }
        // cout<<"The min_edges after iteration:\n";
        // cout<<"The rep vertices are\n";
        // for (int i = 0; i< 7; i++){
        //     // disSet.parent[i] = disSet.Findcompress(i);
        //     cout<<getChar(i)<<'='<<getChar(disSet.parent[i])<<'\n';
        // }
        cout<<'\n';

        printLoop(min_edges);
        //merge components in parallel
#pragma omp parallel for
        for(int i = 0; i < 7; i++) {
            if (min_edges[i] == INT_MAX)
                continue;
            //find the minimum outgoing neighbour
            int outNeighbour = edgeWeightArray[2 * min_edges[i]];
            int outNeighbourWeight = edgeWeightArray[2 *  min_edges[i] + 1];
            int repVertex = disSet.Findcompress(i);
            int repOutNeighbour = disSet.Findcompress(outNeighbour);
            if (repVertex != repOutNeighbour){
                disSet.Merge(repVertex,repOutNeighbour); 
                minimumSpanningTree.push_back(
                        Edge(i, outNeighbour, outNeighbourWeight)
                        );
#pragma omp atomic
                numOfComponents--;
            }
        }
    }
    printEdges(minimumSpanningTree);
}
void readGraph(){
    ifstream graph("./sample_graph.txt");
    string x;
    int count = 0;
    while(graph){
        if(count < 8){
            getline(graph,x);
            indexArray[count] = stoi(x);
        }
        else if(count < 56){
            getline(graph,x);
            edgeWeightArray[count - 8] = x[0]-'a';
            if(x.size() == 3)
                edgeWeightArray[count-7] = x[2]-'0';
            else if(x.size() == 4)
                edgeWeightArray[count-7] = (x[2]-'0')*10+x[3]-'0';
            count++;
        }
        else break;
        count++;
    }
}

void printLoop(vector<int> &arr){
    for(auto it: arr){
        cout<<it<<' ';
    }
    cout<<'\n';
}
void getEdges(char vertex){
    int vertexNumber = vertex-'a';
    for(int i=indexArray[vertexNumber]; i<indexArray[vertexNumber+1] ;i++){
        cout<<(char)(edgeWeightArray[2*i]+'a')<<' ';
        cout<<edgeWeightArray[2*i+1]<<' ';
    }
    cout<<'\n';
}

void printEdges(vector<Edge> & mst){
    for(auto it: mst){
        cout<<getChar(it.in)<<' '<<getChar(it.out)<<' '<<it.edgeWeight<<endl;
    }
}

char getChar(int a){
    return char(a+'a');
}

int getMinEdge(int vertexNumber){
    int repVertex = disSet.Findcompress(vertexNumber);
    int startingEdge = indexArray[vertexNumber];
    int endingEdge = indexArray[vertexNumber + 1];
    int minEdgeWeight = INT_MAX;
    int minEdgeIndex = -1;
    for(int i = startingEdge; i < endingEdge ; i++){
        int outNeighbour = edgeWeightArray[2 * i];
        int outNeighbourEdgeWeight = edgeWeightArray[2*i + 1];
        int repOutNeighbour = disSet.Findcompress(outNeighbour);
        if(outNeighbourEdgeWeight < minEdgeWeight && repVertex != repOutNeighbour) {
            minEdgeWeight = outNeighbourEdgeWeight;
            minEdgeIndex = i;
        }
    }
    return minEdgeIndex;
}

bool checkIfSameParent(vector<int> &arr){
    for(int i = 0; i < (int)arr.size() - 1; i++ ){
        if(disSet.Findcompress(arr[i]) != disSet.Findcompress(arr[i+1]))
            return false;
    }
    return true;
}
