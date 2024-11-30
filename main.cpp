#include <iostream>
#include <fstream>
#include<algorithm>
#include<vector>
using namespace std;

void readBinaryGrFile(const string& filePath) {
    ifstream file(filePath, ios::binary);

    if (!file) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    int32_t node1, node2, weight;  // Assuming weight is an integer (adjust to float if needed)

    vector<int> indexArray(10,0);
    vector<vector<int>> edgesArray(10);
    int edgeCount = 0;
    while (file.read(reinterpret_cast<char*>(&node1), sizeof(node1))) {
        file.read(reinterpret_cast<char*>(&node2), sizeof(node2));
        file.read(reinterpret_cast<char*>(&weight), sizeof(weight));

        // Displaying the edge details
        // cout << "Edge: " << node1 << " -> " << node2 << ", Weight: " << weight << endl;
        // cout<<indexArray.size()-1<<' '<<node1<<endl;
        if(node1 > indexArray.size() - 1){
            indexArray.resize((node1 + 1));
            edgesArray.resize((node1 + 1));
        }
        indexArray[node1]++;
        edgesArray[node1].push_back(node2);
        edgesArray[node1].push_back(weight);
        edgeCount++;
    }
    vector<int> edgeWeightArray;
    for (auto vertex: edgesArray)
        for (auto outNeighbours: vertex)
            edgeWeightArray.push_back(outNeighbours);
    edgesArray = vector<vector<int>>(); //deleting the old array to save memory
    // cout<<indexArray.size()<<' '<<edgesArray.size()<<endl;
    file.close();
    // cout<<"Number of vertices = "<<maxVertexNumber<<" and no: of edges = "<<noOfEdges<<'\n';
}

int main() {
    string graphName;
    cin>>graphName;
    string filePath = "./graphs/"+graphName+".gr";  // Replace with your actual file path
    readBinaryGrFile(filePath);
    // vector<vector<int>> adjList(t+1);

    return 0;
}
