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

        Edge(){
            in = -1;
            out = -1;
            edgeWeight = INT_MAX;
        }

        Edge(int u, int v, int weight){
            in = u;
            out = v;
            edgeWeight = weight;
        }
};

int noOfVertices;
int noOfEdges;
vector<int> indexArray;
vector<int> edgeWeightArray;
HSet disSet;
vector<Edge> minimumSpanningTree;
int prevNoOfComponents = -1;


/*
   function declarations 
   */

void readGraphinCSR(string);
void getEdges(char);
void printLoop(vector<int>&);
void printEdges(vector<Edge>&);
char getChar(int);
int getMinEdge(int);
bool checkIfSameParent(vector<int>&);
int getVertexFromEdge(int);

int main(){
    readGraphinCSR("./graphs/x2.gr");
    int i;
    disSet.Allocate(noOfVertices);
    cout<<"The edges of MST are:"<<endl;

    int counter = 0;
    int numOfComponents = noOfVertices;
    while(numOfComponents > 1){
        vector<int> min_edges(noOfVertices, INT_MAX);
        map<pair<int,int>,Edge> componentsToMerge;
#pragma omp parallel for
        for(int i = 0; i < noOfVertices; i++){
            int repVertex = disSet.Findcompress(i);
            int minEdgeIndex = getMinEdge(i);
            if (minEdgeIndex == -1) 
                continue;
            int minEdgeWeight = edgeWeightArray[2* minEdgeIndex + 1];
            int minWeightOutNeighbor = edgeWeightArray [2* minEdgeIndex];
            // cout<<getChar(i)<<' '<<getChar(minWeightOutNeighbor)<<' '<<minEdgeWeight<<endl;
            //use critical to update the min_edges
#pragma omp critical
            {
                if(min_edges[repVertex] == INT_MAX || minEdgeWeight < edgeWeightArray[2*min_edges[repVertex]+1]) {
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
        // cout<<'\n';

        // printLoop(min_edges);
        //merge components in parallel
#pragma omp parallel for
        for(int i = 0; i < noOfVertices; i++) { 
            if (min_edges[i] == INT_MAX)
                continue;
            int temp_i = getVertexFromEdge(min_edges[i]);
            //find the minimum outgoing neighbour
            int outNeighbour = edgeWeightArray[2 * min_edges[i]];
            int outNeighbourWeight = edgeWeightArray[2 *  min_edges[i] + 1];
            int repVertex = disSet.Findcompress(i);
            int repOutNeighbour = disSet.Findcompress(outNeighbour);
            if (repVertex != repOutNeighbour){
                if (repOutNeighbour < repVertex) {
                    swap(repVertex, repOutNeighbour);
                    swap(temp_i, outNeighbour);
                }
// #pragma omp critical
//                 {
                // cout<<getChar(temp_i)<<' '<<getChar(outNeighbour)<<' '<<outNeighbourWeight<<endl;
//                 }
// #pragma omp critical
                // {
                if(componentsToMerge.find({repVertex,repOutNeighbour}) != componentsToMerge.end()){
                    if(outNeighbourWeight < componentsToMerge[{repVertex,repOutNeighbour}].edgeWeight){
                        componentsToMerge[{repVertex,repOutNeighbour}] = Edge(temp_i, outNeighbour, outNeighbourWeight);
                    }
                }
                else
                {
                    componentsToMerge[{repVertex,repOutNeighbour}] = Edge(temp_i, outNeighbour, outNeighbourWeight);
                }
                // }
            }
            // cout<<i<<endl;
        }
        cout<<"breakpoint\n";
        for(auto componentEdgeMap: componentsToMerge){
            auto component = componentEdgeMap.first;
            auto edge = componentEdgeMap.second;
            // cout<<"Component = "<<getChar(component.first)<<' '<<getChar(component.second)<<endl;
            // cout<<"Edge = "<<getChar(edge.in)<< ' '<< getChar(edge.out)<<' '<<edge.edgeWeight<<endl;

            // disSet.Merge(repVertex,repOutNeighbour); 
            if(disSet.Findcompress(component.first) == disSet.Findcompress(component.second))
                continue;
            disSet.Merge(component.first, component.second);
            minimumSpanningTree.push_back(edge);
            numOfComponents--;
        }

        // if(numOfComponents == 3542490) break;
// #pragma omp atomic
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
                edgeWeightArray[count-noOfVertices] = x[2]-'0';
            else if(x.size() == 4)
                edgeWeightArray[count-noOfVertices] = (x[2]-'0')*10+x[3]-'0';
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
        if (it.out != -1 && it.in != -1)
        cout<<(it.in)<<' '<<(it.out)<<' '<<it.edgeWeight<<endl;
    }
}

char getChar(int a){
    return char(a+'a');
}

int getVertexFromEdge(int edgeNo){
    int flag = 0;
    int i;
    for(i =0; i< noOfVertices;i++){
        if(edgeNo <indexArray[i] ){
            flag = 1;
            break;
        }
    }
    return ((flag==0)?noOfVertices:(i-1));
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


void readGraphinCSR(string filePath) {
    ifstream file(filePath, ios::binary);

    if (!file) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    int32_t node1, node2, weight;  // Assuming weight is an integer (adjust to float if needed)

    vector<int>noOfEdgesPerVertex(10,0);
    vector<vector<int>> edgesArray(10);
    while (file.read(reinterpret_cast<char*>(&node1), sizeof(node1))) {
        file.read(reinterpret_cast<char*>(&node2), sizeof(node2));
        file.read(reinterpret_cast<char*>(&weight), sizeof(weight));

        // Displaying the edge details
        // cout << "Edge: " << node1 << " -> " << node2 << ", Weight: " << weight << endl;
        // cout<<indexArray.size()-1<<' '<<node1<<endl;
        if(node1 > noOfEdgesPerVertex.size() - 1){
            noOfEdgesPerVertex.resize((node1 + 1));
            edgesArray.resize((node1 + 1));
        }
        noOfEdgesPerVertex[node1]++;
        edgesArray[node1].push_back(node2);
        edgesArray[node1].push_back(weight);
        noOfEdges ++;
    }
    for (auto vertex: edgesArray)
        for (auto outNeighbours: vertex)
            edgeWeightArray.push_back(outNeighbours);
    edgesArray = vector<vector<int>>(); //deleting the old array to save memory
    indexArray.push_back(noOfEdgesPerVertex[0]);
    for(int i =1; i< noOfEdgesPerVertex.size();i++){
        indexArray.push_back(indexArray[i-1] + noOfEdgesPerVertex[i]);
    }

    noOfVertices = indexArray.size();
    file.close();
    // cout<<"Number of vertices = "<<maxVertexNumber<<" and no: of edges = "<<noOfEdges<<'\n';
}
