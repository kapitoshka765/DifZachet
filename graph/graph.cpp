#include "graph.h"
#include <QMessageBox>
#include <queue>
#include <stack>
using namespace std;

Graph::Graph(int vertices) {
    numVertices = vertices;

    adjacencyMatrix.resize(numVertices, vector<int>(numVertices, 0));
    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i][i] = 0;
    }
}

int Graph::getNumVertices() const {
    return numVertices;
}

int Graph::getEdgeWeight(int v1, int v2) const {
    return adjacencyMatrix[v1][v2];
}

void Graph::addEdge(int v1, int v2, int weight) {
    adjacencyMatrix[v1][v2] = weight;
    adjacencyMatrix[v2][v1] = weight;
}

vector<int> Graph::getVertices() const {
    vector<int> vertices(numVertices);
    for (int i = 0; i < numVertices; i++) {
        vertices[i] = i;
    }
    return vertices;
}

void Graph::addVertex() {
    numVertices++;
    adjacencyMatrix.resize(numVertices, vector<int>(numVertices, 0));
    for (int i = 0; i < numVertices - 1; i++) {
        adjacencyMatrix[i].resize(numVertices);
    }
}

void Graph::removeVertex(int vertex) {
    if (vertex < 0 || vertex >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверный номер вершины");
        return;
    }
    numVertices--;

    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i].erase(adjacencyMatrix[i].begin() + vertex);
    }

    adjacencyMatrix.erase(adjacencyMatrix.begin() + vertex);
}

vector<std::vector<int>> Graph::getAdjacencyMatrix() const {
    return adjacencyMatrix;
}


void Graph::removeEdge(int v1, int v2) {
    if (v1 < 0 || v1 >= numVertices || v2 < 0 || v2 >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверные номера вершин");
        return;
    }
    adjacencyMatrix[v1][v2] = 0;
    adjacencyMatrix[v2][v1] = 0;
}


void Graph::editEdgeWeight(int v1, int v2, int weight) {
    if (v1 < 0 || v1 >= numVertices || v2 < 0 || v2 >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверные номера вершин");
        return;
    }
    adjacencyMatrix[v1][v2] = weight;
    adjacencyMatrix[v2][v1] = weight;
}

void Graph::breadthFirstSearch(int startVertex) const {
    if (startVertex < 0 || startVertex >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверный номер вершины");
        return;
    }

    vector<bool> visited(numVertices, false);
    queue<int> queue;

    visited[startVertex] = true;
    queue.push(startVertex);

    QString result;

    while (!queue.empty()) {
        int currentVertex = queue.front();
        queue.pop();
        result += QString::number(currentVertex) + " ";

        for (int i = 0; i < numVertices; i++) {
            if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i]) {
                visited[i] = true;
                queue.push(i);
            }
        }
    }

    QMessageBox::information(nullptr, "Результат", result);
}

void Graph::depthFirstSearch(int startVertex) const {
    if (startVertex < 0 || startVertex >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверный номер вершины");
        return;
    }

    vector<bool> visited(numVertices, false);
    stack<int> stack;

    visited[startVertex] = true;
    stack.push(startVertex);

    QString output;

    while (!stack.empty()) {
        int currentVertex = stack.top();
        stack.pop();
        output += QString::number(currentVertex) + " ";

        for (int i = 0; i < numVertices; i++) {
            if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i]) {
                visited[i] = true;
                stack.push(i);
            }
        }
    }

    QMessageBox::information(nullptr, "Результат", output);
}

void Graph::Dijkstra(int startVertex, Graph graph) const {
    vector<int> shortestDistances(numVertices, INT_MAX);
    vector<bool> visited(numVertices, false);

    shortestDistances[startVertex] = 0;

    for (int i = 0; i < numVertices - 1; i++) {
        int minDistance = INT_MAX;
        int minIndex = -1;

        for (int j = 0; j < numVertices; j++) {
            if (!visited[j] && shortestDistances[j] <= minDistance) {
                minDistance = shortestDistances[j];
                minIndex = j;
            }
        }

        visited[minIndex] = true;

        for (int j = 0; j < numVertices; j++) {
            if (!visited[j] && graph.getEdgeWeight(minIndex, j) > 0 && shortestDistances[minIndex] != INT_MAX &&
                shortestDistances[minIndex] + graph.getEdgeWeight(minIndex, j) < shortestDistances[j]) {
                shortestDistances[j] = shortestDistances[minIndex] + graph.getEdgeWeight(minIndex, j);
            }
        }
    }

    for (int i = 0; i < numVertices; i++) {
        QString message = "Кратчайшее расстояние от вершины " + QString::number(startVertex) + " до вершины " + QString::number(i) + ": ";
        if (shortestDistances[i] == INT_MAX) {
            message += "нет пути";
        } else {
            message += QString::number(shortestDistances[i]);
        }

        QMessageBox::information(nullptr, "Результат", message);
    }
}
