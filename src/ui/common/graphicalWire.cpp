/*
  Copyright (C) 2025 Giulio Cocconi

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "graphicalWire.hpp"

GraphicalWireJunction::GraphicalWireJunction(QPoint point, QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptedMouseButtons(Qt::AllButtons);

  this->point = point;
}

QRectF GraphicalWireJunction::boundingRect() const
{
  return QRectF(-RADIUS, -RADIUS, RADIUS * 2, RADIUS * 2);
}

void GraphicalWireJunction::paint(QPainter*                       painter,
                                  const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->drawEllipse(boundingRect());
}


GraphicalWire::GraphicalWire(std::vector<GraphicalWireSegment*> segments,
                             QGraphicsItem*                     parent)
  : QGraphicsItem(parent)
{
  for (auto seg : segments)
    addSegment(seg);
}

void GraphicalWire::addSegment(GraphicalWireSegment* segment)
{
  segments.push_back(segment);

  sort(segments.begin(), segments.end());
  segments.erase(unique(segments.begin(), segments.end()), segments.end());
}

void GraphicalWire::removeSegment(GraphicalWireSegment* segment) {

  const auto b   = segments.begin();
  const auto e   = segments.end();
  const auto pos = std::find(b, e, segment);

  if (pos != e)
    segments.erase(pos);
}


QRectF GraphicalWire::boundingRect() const {
  QRectF rect {};

  for (QGraphicsItem* child : childItems())
    rect = rect.united(child->boundingRect());

  return rect;
}

bool GraphicalWire::isPointOnPath(QPointF point) {

  for (auto segment : segments)
    if (segment->isPointOnPath(point))
      return true;

  return false;

}

GraphicalWireSegment::GraphicalWireSegment(QPointF firstPoint, QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  points.push_back(firstPoint);
  updatePath();
}

void GraphicalWireSegment::setGraphicalWire(GraphicalWire* graphicalWire) {
  setParentItem(graphicalWire);
  this->graphicalWire = graphicalWire;
}

void GraphicalWireSegment::setShowPoints(std::vector<QPointF> points)
{
  assert(points.size() <= 2);

  // TODO: Check for collision with items but not with ports!

  this->showPoints = points;

  updatePath();
}

void GraphicalWireSegment::addPoints()
{
  if (points.empty()) {
    points = std::move(showPoints);

    this->showPoints = {};

    updatePath();
    return;
  }

  // Check for self intersecting
  QPainterPathStroker stroker;

  auto showStroke = stroker.createStroke(showPath);

  auto intersection = shape().intersected(showStroke);

  // Exclude the last point of path
  QPainterPath exclusionZone;
  exclusionZone.addEllipse(lastPoint(), 1, 1);

  intersection = intersection.subtracted(exclusionZone);

  if (intersection.isEmpty()) {
    for (auto pt : this->showPoints)
      this->points.push_back(pt);
    this->showPoints = {};
    updatePath();
  } else {
    qDebug() << "Self intersecting";
  }
}

void GraphicalWireSegment::updatePath()
{
  prepareGeometryChange();
  path.clear();
  showPath.clear();

  // Set initial position
  path.moveTo(this->points[0]);

  // Draw definitive points
  for (int i = 1; i < this->points.size(); i++)
    path.lineTo(this->points[i]);

  // Draw showpoints
  if (!this->showPoints.empty()) {
    showPath.moveTo(this->lastPoint());

    for (int i = 0; i < this->showPoints.size(); i++) {
      showPath.lineTo(this->showPoints[i]);
    }
  }
}

void GraphicalWireSegment::paint(QPainter*                       painter,
                                 const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->setPen(QPen(Qt::blue, 3));
  painter->drawPath(path);

  painter->setPen(QPen(Qt::red, 3));
  painter->drawPath(showPath);
}

// FIXME: The path shape behaves like the path is closed even if it's not.

QRectF GraphicalWireSegment::boundingRect() const
{
  return (this->path.boundingRect())
      .united(this->showPath.boundingRect())
      .adjusted(-5, -5, 5, 5);
}

QPainterPath GraphicalWireSegment::shape() const
{
  QPainterPathStroker stroker;
  return stroker.createStroke(this->path);
}

bool GraphicalWireSegment::isPointOnPath(const QPointF point)
{
  // Assertions needed to make it work:
  // 1) Manhattan-style routing system
  // 2) Points aligned to int-sized grid

  if (points.size() == 0)
    return false;

  if (points.size() == 1)
    return (point.x() == points[0].x() && point.y() == points[0].y());

  const auto slide_view = points | std::views::slide(2);

  // For each sub-segment
  for (const auto el : slide_view) {
    const bool horizontalSegment = (el[0].y() == el[1].y());
    if (horizontalSegment && el[0].y() == point.y()) {
      const auto minX = std::min(el[0].x(), el[1].x());
      const auto maxX = std::max(el[0].x(), el[1].x());

      if (point.x() >= minX && point.x() <= maxX)
        return true;
    }

    const bool verticalSegment = (el[0].x() == el[1].x());
    if (verticalSegment && el[0].x() == point.x()) {
      const auto minY = std::min(el[0].y(), el[1].y());
      const auto maxY = std::max(el[0].y(), el[1].y());

      if (point.y() >= minY && point.y() <= maxY)
        return true;
    }
  }

  return false;
}

GraphicalWireSegment::~GraphicalWireSegment()
{
  graphicalWire->removeSegment(this);
}
