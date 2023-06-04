#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRectF>
#include <QGraphicsTextItem>
#include "graph.h"

using namespace std;

struct PathInfo {
    std::vector<int> path;
    int cost;

    PathInfo(const std::vector<int>& _path, int _cost) : path(_path), cost(_cost) {}
};

class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    GraphWidget(QWidget* parent = nullptr) : QGraphicsView(parent) {
        setRenderHint(QPainter::Antialiasing);
        setScene(new QGraphicsScene(this));
    }

    void reshGraph(const Graph& graph, const PathInfo& optimalPath);
    void visGraph(const Graph& graph);

private:
    void drawEdges(const Graph& graph, const vector<QPointF>& vertexPositions);
    void drawVertices(const vector<QPointF>& vertexPositions);
    void drawPath(const vector<int>& path, const vector<QPointF>& vertexPositions);
    const int vertexLabelOffset = 5;
};

#endif // GRAPHWIDGET_H
