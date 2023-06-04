#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
private:
    int numVertices;
    std::vector<std::vector<int>> adjacencyMatrix;

public:
    Graph(int vertices);

    void addVertex();
    void removeVertex(int vertex);
    void removeEdge(int v1, int v2);
    void editEdgeWeight(int v1, int v2, int weight);
    int getNumVertices() const;
    int getEdgeWeight(int v1, int v2) const;
    void addEdge(int v1, int v2, int weight);
    std::vector<int> getVertices() const;
    std::vector<std::vector<int>> getAdjacencyMatrix() const;
};

#endif // GRAPH_H
