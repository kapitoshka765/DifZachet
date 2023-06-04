#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwidget.h"
#include "graph.h"
#include "qpushbutton.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onVertexCountChanged(const QString& text);
    void updateGraph(int vertexCount);
    void showAdjacencyMatrix();
    void addVertex();
    void resh();
    void addEdge();
    void removeEdge();
    void removeVertex();
    void editWeight();

private:
    Ui::MainWindow *ui;
    GraphWidget *graphWidget; // Указатель на виджет графа
    Graph graph; // Граф
    int startVertex; // Начальная вершина для задачи Коммивояжера
    QPushButton* reshButton;
    QLineEdit* vertexCountLineEdit;
    QPushButton* adjacencyMatrixButton;
    QPushButton* addVertexButton;
    QPushButton* addEdgeButton;
    QPushButton* removeEdgeButton;
    QPushButton* removeVertexButton;
    QPushButton* editEdgeWeightButton;
};


#endif // MAINWINDOW_H
