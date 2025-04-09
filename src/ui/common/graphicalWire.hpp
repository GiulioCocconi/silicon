#pragma once

#include <cassert>
#include <vector>

#include <QPoint>
#include <QPainterPath>
#include <QGraphicsItem>

#include <core/wire.hpp>
#include <ui/common/diagramView.hpp>



class GraphicalWire : public QGraphicsItem {
public:
  GraphicalWire(std::vector<QPoint> points, QGraphicsItem* parent = nullptr);
  GraphicalWire(QPainterPath path, Bus bus, QGraphicsItem* parent = nullptr);
  void updatePoints(std::vector<QPoint> points);
  void addPoint(QPoint point);
  
  void setBus(Bus bus);
  Bus getBus();
  
private:
  QPainterPath path;

  Bus bus;
  
};
