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

GraphicalWire::GraphicalWire(std::vector<GraphicalWireSegment*> segments,
                             QGraphicsItem*                     parent)
  : QGraphicsItem(parent)
{
  for (auto seg : segments)
    addSegment(seg);
}

void GraphicalWire::addSegment(GraphicalWireSegment* segment)
{
  prepareGeometryChange();
  segments.push_back(segment);

  sort(segments.begin(), segments.end());
  segments.erase(unique(segments.begin(), segments.end()), segments.end());
}

void GraphicalWire::removeSegment(GraphicalWireSegment* segment)
{
  const auto b   = segments.begin();
  const auto e   = segments.end();


  // If pointers are not valid then we can safely return. The segment won't be there anyway
  if (!b.base() || !e.base())
    return;

  const auto pos = std::find(b, e, segment);

  if (pos != e) {
    prepareGeometryChange();
    segments.erase(pos);
  }
}

QRectF GraphicalWire::boundingRect() const
{
  QRectF rect{};

  for (QGraphicsItem* child : childItems())
    rect = rect.united(child->boundingRect());

  return rect;
}

void GraphicalWire::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget)
{
  // Draw junctions

  painter->setPen(QPen(Qt::blue, 3));
  painter->setBrush(Qt::blue);

  const auto b = this->segments.begin();
  const auto e = this->segments.end();

  // Unordered pairs of segments
  for (auto first = b; first != e; ++first) {
    for (auto second = first + 1; second != e; ++second) {
      qDebug() << *first << "\n" << *second;
      const QPointF firstPoint = (*second)->firstPoint();
      const QPointF lastPoint  = (*second)->lastPoint();

      const bool firstPointIntersects = (*first)->isPointOnPath(firstPoint);
      const bool lastPointIntersects  = (*first)->isPointOnPath(lastPoint);

      if (firstPointIntersects || lastPointIntersects) {
        painter->drawEllipse(firstPointIntersects ? firstPoint : lastPoint, 5, 5);
      }
    }
  }
}

GraphicalWireSegment* GraphicalWire::segmentAtPoint(QPointF point)
{
  for (auto segment : segments) {
    QPointF segmentPoint = segment->mapFromScene(point);
    if (segment->isPointOnPath(segmentPoint)) {
      qDebug() << segment;
      return segment;
    }
  }

  return nullptr;
}

GraphicalWireSegment::GraphicalWireSegment(QPointF firstPoint, QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  points.push_back(firstPoint);
  updatePath();
}

void GraphicalWireSegment::setGraphicalWire(GraphicalWire* graphicalWire)
{
  setParentItem(graphicalWire);
  graphicalWire->addSegment(this);
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
  // Add a small tolerance for point detection
  const double tolerance = 5.0;  // Adjust based on your needs

  if (points.size() == 0)
    return false;

  if (points.size() == 1)
    return QLineF(point, points[0]).length() <= tolerance;

  const auto slide_view = points | std::views::slide(2);

  // For each sub-segment
  for (const auto el : slide_view) {
    const bool horizontalSegment = (qAbs(el[0].y() - el[1].y()) <= tolerance);
    if (horizontalSegment && qAbs(el[0].y() - point.y()) <= tolerance) {
      const auto minX = std::min(el[0].x(), el[1].x());
      const auto maxX = std::max(el[0].x(), el[1].x());

      if (point.x() >= minX - tolerance && point.x() <= maxX + tolerance)
        return true;
    }

    const bool verticalSegment = (qAbs(el[0].x() - el[1].x()) <= tolerance);
    if (verticalSegment && qAbs(el[0].x() - point.x()) <= tolerance) {
      const auto minY = std::min(el[0].y(), el[1].y());
      const auto maxY = std::max(el[0].y(), el[1].y());

      if (point.y() >= minY - tolerance && point.y() <= maxY + tolerance)
        return true;
    }
  }

  return false;
}

GraphicalWireSegment::~GraphicalWireSegment()
{
  if (graphicalWire)
    graphicalWire->removeSegment(this);
}
