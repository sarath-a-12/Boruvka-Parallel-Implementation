#include <iostream>
#include <fstream>
#include<algorithm>
using namespace std;

void readBinaryGrFile(const string& filePath) {
    ifstream file(filePath, ios::binary);

    if (!file) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    int32_t node1, node2, weight;  // Assuming weight is an integer (adjust to float if needed)

    int maxVertexNumber = -1, noOfEdges = 0;
    while (file.read(reinterpret_cast<char*>(&node1), sizeof(node1))) {
        file.read(reinterpret_cast<char*>(&node2), sizeof(node2));
        file.read(reinterpret_cast<char*>(&weight), sizeof(weight));

        // Displaying the edge details
        cout << "Edge: " << node1 << " -> " << node2 << ", Weight: " << weight << endl;
        noOfEdges ++ ;
        if(node1 > maxVertexNumber) maxVertexNumber = node1;
        if(node2 > maxVertexNumber) maxVertexNumber = node2;
    }
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
