#include "graph.h"
#include "graphwidget.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRectF>

void GraphWidget::reshGraph(const Graph& graph, const PathInfo& optimalPath) {


    scene()->clear();

    int numVertices = graph.getNumVertices();

    const int sceneWidth = 1000;
    const int sceneHeight = 1000;
    const QPointF center(sceneWidth / 2, sceneHeight / 2);
    const qreal radius = qMin(sceneWidth, sceneHeight) * 0.4;
    const qreal angleIncrement = 2 * M_PI / numVertices;

    std::vector<QPointF> vertexPositions;
    for (int i = 0; i < numVertices; i++) {
        qreal angle = i * angleIncrement;
        qreal x = center.x() + radius * qCos(angle);
        qreal y = center.y() + radius * qSin(angle);
        vertexPositions.emplace_back(x, y);
    }

    drawEdges(graph, vertexPositions);

    drawVertices(vertexPositions);

    drawPath(optimalPath.path, vertexPositions);
}

void GraphWidget::visGraph(const Graph& graph)
{
    scene()->clear();

    int numVertices = graph.getNumVertices();

    const int sceneWidth = 1000;
    const int sceneHeight = 1000;
    const QPointF center(sceneWidth / 2, sceneHeight / 2);
    const qreal radius = qMin(sceneWidth, sceneHeight) * 0.4;
    const qreal angleIncrement = 2 * M_PI / numVertices;

    std::vector<QPointF> vertexPositions;
    for (int i = 0; i < numVertices; i++) {
        qreal angle = i * angleIncrement;
        qreal x = center.x() + radius * qCos(angle);
        qreal y = center.y() + radius * qSin(angle);
        vertexPositions.emplace_back(x, y);
    }

    drawEdges(graph, vertexPositions);

    drawVertices(vertexPositions);
}

void GraphWidget::drawEdges(const Graph& graph, const std::vector<QPointF>& vertexPositions) {
    const int numVertices = graph.getNumVertices();
    const QPen edgePen(Qt::black);

    for (int v1 = 0; v1 < numVertices; v1++) {
        for (int v2 = 0; v2 < numVertices; v2++) {
            int weight = graph.getEdgeWeight(v1, v2);
            if (weight > 0) {
                QPointF p1 = vertexPositions[v1];
                QPointF p2 = vertexPositions[v2];
                scene()->addLine(p1.x(), p1.y(), p2.x(), p2.y(), edgePen);
            }
        }
    }
}

void GraphWidget::drawVertices(const std::vector<QPointF>& vertexPositions) {
    const int numVertices = vertexPositions.size();
    const int vertexRadius = 10;
    const QPen vertexPen(Qt::black);
    const QBrush vertexBrush(Qt::blue);

    for (int v = 0; v < numVertices; v++) {
        QPointF position = vertexPositions[v];
        QRectF rect(position.x() - vertexRadius, position.y() - vertexRadius,
                    2 * vertexRadius, 2 * vertexRadius);
        scene()->addEllipse(rect, vertexPen, vertexBrush);

        QGraphicsTextItem* label = scene()->addText(QString::number(v));
        label->setPos(position.x() - vertexLabelOffset, position.y() - vertexLabelOffset);
    }
}

void GraphWidget::drawPath(const std::vector<int>& path, const std::vector<QPointF>& vertexPositions) {
    if (path.empty())
        return;

    const int pathPenWidth = 2;
    const QColor pathColor = Qt::red;
    const QPen pathPen(pathColor, pathPenWidth);

    const int numVertices = path.size();
    for (int i = 0; i < numVertices - 1; i++) {
        int v1 = path[i];
        int v2 = path[i + 1];
        QPointF p1 = vertexPositions[v1];
        QPointF p2 = vertexPositions[v2];
        scene()->addLine(p1.x(), p1.y(), p2.x(), p2.y(), pathPen);
    }

    int v1 = path.back();
    int v2 = path.front();
    QPointF p1 = vertexPositions[v1];
    QPointF p2 = vertexPositions[v2];
    scene()->addLine(p1.x(), p1.y(), p2.x(), p2.y(), pathPen);
}
