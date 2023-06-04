#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <queue>
#include <QMessageBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QInputDialog>
#include <QVBoxLayout>

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), graph(0)
{
    ui->setupUi(this);


    // Создание графического виджета и добавление его на главное окно
    graphWidget = new GraphWidget(this);
    graphWidget->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));
    // Создание кнопки и установка текста
    reshButton = new QPushButton("Решение", this);
    // Перемещение кнопки в нужное положение
    // Подключение сигнала нажатия кнопки к слоту resh()
    connect(reshButton, &QPushButton::clicked, this, &MainWindow::resh);
    reshButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey; text-align: center"));

    // Создание поля для ввода числа вершин
    vertexCountLineEdit = new QLineEdit(this);
    vertexCountLineEdit->setGeometry(100,100,300,100);
    vertexCountLineEdit->setPlaceholderText("Введите число вершин");
    vertexCountLineEdit->setValidator(new QIntValidator(2, 100, this));
    vertexCountLineEdit->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey; text-align: center"));


    // Подключение сигнала изменения текста в поле для ввода
    connect(vertexCountLineEdit, &QLineEdit::textChanged, this, &MainWindow::onVertexCountChanged);

    // Добавление поля для ввода в главное окно

    adjacencyMatrixButton = new QPushButton("Матрица смежности",this);
    connect(adjacencyMatrixButton, &QPushButton::clicked, this, &MainWindow::showAdjacencyMatrix);
    adjacencyMatrixButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));

    addVertexButton = new QPushButton("Добавить вершину", this);
    connect(addVertexButton, &QPushButton::clicked, this, &MainWindow::addVertex);
    addVertexButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));


    addEdgeButton = new QPushButton("Добавить ребро", this);
    connect(addEdgeButton, &QPushButton::clicked, this, &MainWindow::addEdge);
    addEdgeButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));

    removeEdgeButton = new QPushButton("Удалить ребро", this);
    connect(removeEdgeButton, &QPushButton::clicked, this, &MainWindow::removeEdge);
    removeEdgeButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));

    removeVertexButton = new QPushButton("Удалить вершину", this);
    connect(removeVertexButton, &QPushButton::clicked, this, &MainWindow::removeVertex);
    removeVertexButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));

    editEdgeWeightButton = new QPushButton("Изменить вес ребра", this);
    connect(editEdgeWeightButton, &QPushButton::clicked, this, &MainWindow::editWeight);
    editEdgeWeightButton->setStyleSheet(QString("background-color: LightBlue; border-style: outset; border-width: 2px; border-color: grey"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(graphWidget);
    layout->addWidget(vertexCountLineEdit);
    layout->addWidget(addVertexButton);
    layout->addWidget(removeVertexButton);
    layout->addWidget(addEdgeButton);
    layout->addWidget(removeEdgeButton);
    layout->addWidget(editEdgeWeightButton);
    layout->addWidget(reshButton);
    layout->addWidget(adjacencyMatrixButton);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    widget->setStyleSheet(QString("background-color: DimGrey"));
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

const int INF = numeric_limits<int>::max(); // Бесконечность

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
            int minRemainingEdgeWeight = INF;
            for (int vertex : remainingVertices) {
                int remainingEdgeWeight = graph.getEdgeWeight(nextVertex, vertex);
                if (remainingEdgeWeight < minRemainingEdgeWeight)
                    minRemainingEdgeWeight = remainingEdgeWeight;
            }
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

//Функция, которая выводит на экран решение задачи Коммивояжера методом ветвей и границ
void MainWindow::resh()
{
    int vertexCount = graph.getNumVertices();
    startVertex = QInputDialog::getInt(this, "Стартовая вершина", "Введите номер вершины", 0, 0, vertexCount - 1, 1);
    // Решение задачи Коммивояжера
    PathInfo optimalPath = solveTSP(graph, startVertex);

    // Вывод результатов
    if (!optimalPath.path.empty()) {
        QString optimalPathString = "Оптимальный путь: ";
        for (int vertex : optimalPath.path) {
            optimalPathString += QString::number(vertex) + " ";
        }
        optimalPathString += QString::number(startVertex);

        QString costString = "Стоимость: " + QString::number(optimalPath.cost);

        QMessageBox::information(this, "Результаты", optimalPathString + "\n" + costString);

        // Визуализация графа и оптимального пути
        graphWidget->reshGraph(graph, optimalPath);
    }
    else {
        QMessageBox::information(this, "Результаты", "Не удалось найти оптимальный путь.");
    }


}

// Функция, которая вызывается при изменении числа вершин
void MainWindow::onVertexCountChanged(const QString& text)
{
    bool ok;
    int vertexCount = text.toInt(&ok);

    if (ok) {
        // Изменение графа
        updateGraph(vertexCount);
    } else {
        // Ввод некорректного значения
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите целое число вершин.");
    }
}

// Функция, которая обновляет граф при изменении числа вершин
void MainWindow::updateGraph(int vertexCount)
{
    graph = Graph(vertexCount);

    if (vertexCount < 2) {
        QMessageBox::warning(this, "Ошибка", "Число вершин должно быть не менее 2.");
        return;
    }
    bool ok = true;
    while(ok){
        int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, vertexCount - 1, 1, &ok);
        int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, vertexCount - 1, 1, &ok);
        int weight = QInputDialog::getInt(this, "Вес ребра", "Введите вес ребра", 0, 0, std::numeric_limits<int>::max(), 1, &ok);
        graph.addEdge(startikVertex, endVertex, weight);
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Создание ребра", "Хотите создать еще одно ребро?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            ok = false;
        }
    }
    graphWidget->visGraph(graph);
}

// Функция, которая выводит матрицу смежности на экран
void MainWindow::showAdjacencyMatrix()
{
    int numVertices = graph.getNumVertices();
    std::vector<std::vector<int>> adjacencyMatrix = graph.getAdjacencyMatrix();

    QString matrixString;

    // Формирование строки с матрицей смежности
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            matrixString += QString::number(adjacencyMatrix[i][j]) + "\t";
        }
        matrixString += "\n";
    }

    // Отображение матрицы смежности в всплывающем окне
    QMessageBox::information(this, "Матрица смежности", matrixString);
}

// Функция, которая позволяет добавить вершину в граф
void MainWindow::addVertex() {
    graph.addVertex();
    updateGraph(graph.getNumVertices());
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет добавить ребро в граф
void MainWindow::addEdge()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, numVertices - 1, 1);
    int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, numVertices - 1, 1);
    int weight = QInputDialog::getInt(this, "Вес ребра", "Введите вес ребра", 0, 0, std::numeric_limits<int>::max(), 1);
    graph.addEdge(startikVertex, endVertex, weight);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет удалить ребро из графа
void MainWindow::removeEdge()
{
    bool ok = true;
    int numVertices = graph.getNumVertices();
    while(ok){
        int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, numVertices - 1, 1, &ok);
        int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, numVertices - 1, 1, &ok);
        graph.removeEdge(startikVertex, endVertex);
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Удаление ребра", "Хотите удалить еще одно ребро?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            ok = false;
        }
    }
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет удалить вершину из графа
void MainWindow::removeVertex()
{
    int numVertices = graph.getNumVertices();
    int Vertex = QInputDialog::getInt(this, "Вершина", "Введите номер удаляемой вершины", 0, 0, numVertices - 1, 1);
    graph.removeVertex(Vertex);
    graphWidget->visGraph(graph);
}

// Функция, которая позволяет изменить вес ребра
void MainWindow::editWeight()
{
    int numVertices = graph.getNumVertices();
    int startikVertex = QInputDialog::getInt(this, "Начальная вершина", "Введите номер начальной вершины для ребра", 0, 0, numVertices - 1, 1);
    int endVertex = QInputDialog::getInt(this, "Конечная вершина", "Введите номер конечной вершины для ребра", 0, 0, numVertices - 1, 1);
    int weight = QInputDialog::getInt(this, "Новый вес ребра", "Введите новый вес ребра", 0, 0, std::numeric_limits<int>::max(), 1);
    graph.editEdgeWeight(startikVertex, endVertex, weight);
    graphWidget->visGraph(graph);
}
