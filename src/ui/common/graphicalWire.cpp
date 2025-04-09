#include "graphicalWire.hpp"

GraphicalWire::GraphicalWire(std::vector<QPoint> points, QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  this->updatePoints(points);
}


GraphicalWire::GraphicalWire(QPainterPath path, Bus bus,
			     QGraphicsItem* parent) : QGraphicsItem(parent)
{
  this->path = path;
  this->setBus(bus);
}

void GraphicalWire::updatePoints(std::vector<QPoint> points) {
  assert(!points.empty());
  this->path.moveTo(points[0]);
  for (auto point : points)
    this->addPoint(point);
}

void GraphicalWire::addPoint(QPoint point) {
  this->path.lineTo(point);
}

void GraphicalWire::setBus(Bus bus) {
  this->bus = bus;
} 

Bus GraphicalWire::getBus() {
  return this->bus;
}
