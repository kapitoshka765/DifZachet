#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>

using namespace std;

const int INF = numeric_limits<int>::max(); // Бесконечность

// Класс для представления графа
class Graph {
private:
    int numVertices; // Количество вершин
    vector<vector<int>> adjacencyMatrix; // Матрица смежности

public:
    Graph(int vertices) {
        numVertices = vertices;

        // Инициализация матрицы смежности
        adjacencyMatrix.resize(numVertices, vector<int>(numVertices, 0));
        for (int i = 0; i < numVertices; i++) {
            adjacencyMatrix[i][i] = 0; // Вес петли равен 0
        }
    }

    int getNumVertices() const {
        return numVertices;
    }

    int getEdgeWeight(int v1, int v2) const {
        return adjacencyMatrix[v1][v2];
    }

    // Добавление ребра между вершинами v1 и v2 с весом weight
    void addEdge(int v1, int v2, int weight) {
        adjacencyMatrix[v1][v2] = weight;
    }

    // Получение списка вершин графа
    vector<int> getVertices() const {
        vector<int> vertices(numVertices);
        for (int i = 0; i < numVertices; i++) {
            vertices[i] = i;
        }
        return vertices;
    }
};

// Структура для хранения информации о пути
struct PathInfo {
    vector<int> path;
    int cost;

    PathInfo(const vector<int>& _path, int _cost) : path(_path), cost(_cost) {}
};

// Функция для нахождения наименьшего элемента в векторе
int findMin(const vector<int>& arr) {
    int minVal = INF;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

// Функция для нахождения индекса наименьшего элемента в векторе
int findMinIndex(const vector<int>& arr) {
    int minIndex = -1;
    int minVal = INF;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Функция для решения задачи Коммивояжера методом ветвей и границ
PathInfo solveTSP(const Graph& graph, int startVertex) {
    int numVertices = graph.getNumVertices();

    // Инициализация очереди с приоритетом для хранения путей
    priority_queue<PathInfo, vector<PathInfo>, function<bool(const PathInfo&, const PathInfo&)>> pq(
        [](const PathInfo& p1, const PathInfo& p2) {
            return p1.cost > p2.cost; // Сортировка по возрастанию стоимости
        }
        );

    // Инициализация начального пути
    vector<int> path;
    path.push_back(startVertex);
    PathInfo initialPath(path, 0);
    pq.push(initialPath);

    // Инициализация нижней границы стоимости
    int lowerBound = INF;

    while (!pq.empty()) {
        PathInfo currentPath = pq.top();
        pq.pop();

        int currentVertex = currentPath.path.back();
        int currentCost = currentPath.cost;

        // Если путь содержит все вершины, обновляем нижнюю границу и возвращаем путь
        if (currentPath.path.size() == numVertices) {
            int finalCost = currentCost + graph.getEdgeWeight(currentVertex, startVertex);
            return PathInfo(currentPath.path, finalCost);
        }

        // Проверяем все возможные следующие вершины
        vector<int> remainingVertices = graph.getVertices();
        for (int vertex : currentPath.path) {
            remainingVertices.erase(remove(remainingVertices.begin(), remainingVertices.end(), vertex), remainingVertices.end());
        }

        for (int nextVertex : remainingVertices) {
            int edgeWeight = graph.getEdgeWeight(currentVertex, nextVertex);

            // Проверка на невозможность перехода к следующей вершине
            if (edgeWeight == 0)
                continue;

            // Вычисление стоимости пути с добавлением следующей вершины
            int nextCost = currentCost + edgeWeight;

            // Проверка на возможность улучшения нижней границы стоимости
            int minRemainingEdgeWeight = findMinIndex(remainingVertices);
            int lowerBoundCost = nextCost + minRemainingEdgeWeight;
            if (lowerBoundCost >= lowerBound)
                continue;

            // Создание нового пути и добавление его в очередь
            vector<int> newPath = currentPath.path;
            newPath.push_back(nextVertex);
            PathInfo newPathInfo(newPath, nextCost);
            pq.push(newPathInfo);
        }
    }

    // Если не удалось найти оптимальный путь, возвращаем пустой путь и максимальное значение стоимости
    return PathInfo({}, INF);
}
int main() {
    setlocale(0, "RUS");

    // Создание графа
    Graph graph(6);
    // Добавление ребер
    graph.addEdge(0, 2, 13);
    graph.addEdge(0, 3, 15);
    graph.addEdge(1, 3, 20);
    graph.addEdge(2, 4, 30);
    graph.addEdge(3, 5, 31);
    graph.addEdge(3, 4, 39);
    graph.addEdge(4, 3, 39);
    graph.addEdge(4, 1, 21);
    graph.addEdge(5, 0, 18);

    // Выбор начальной вершины
    int startVertex = 0;

    // Решение задачи Коммивояжера
    PathInfo optimalPath = solveTSP(graph, startVertex);

    // Вывод результатов
    if (!optimalPath.path.empty()) {
        cout << "Оптимальный путь: ";
        for (int vertex : optimalPath.path) {
            cout << vertex << " ";
        }
        cout << startVertex << endl;
        cout << "Стоимость: " << optimalPath.cost << endl;
    }
    else {
        cout << "Не удалось найти оптимальный путь." << endl;
    }

    return 0;
}
