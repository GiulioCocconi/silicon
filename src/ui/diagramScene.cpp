#include "diagramScene.hpp"

void DiagramScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen;
    painter->setPen(pen);

    qreal left = int(rect.left()) - (int(rect.left()) % DiagramScene::GRID_SIZE);
    qreal top = int(rect.top()) - (int(rect.top()) % DiagramScene::GRID_SIZE);
    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += DiagramScene::GRID_SIZE){
        for (qreal y = top; y < rect.bottom(); y += DiagramScene::GRID_SIZE){
            points.append(QPointF(x,y));
        }
    }
    painter->drawPoints(points.data(), points.size());
}
