#pragma once
#include <QPainter>
#include <QRect>
#include <QGraphicsScene>

class DiagramScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit DiagramScene(QObject* parent = nullptr) : QGraphicsScene(parent) {}

  static const int GRID_SIZE = 10;
  
private:
  void drawBackground(QPainter *painter, const QRectF &rect) override;
  
};
