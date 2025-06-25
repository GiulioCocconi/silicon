/*
 Copyright (c) 2025. Giulio Cocconi

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

GraphicalWire::GraphicalWire(const std::vector<GraphicalWireSegment*>& segments,
                             QGraphicsItem*                            parent)
  : QGraphicsItem(parent)
{
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);

  // Ensure this item can receive mouse events
  setAcceptedMouseButtons(Qt::AllButtons);

  for (const auto seg : segments)
    addSegment(seg);
}

void GraphicalWire::addSegment(GraphicalWireSegment* segment)
{
  prepareGeometryChange();
  segments.push_back(segment);

  qDebug() << segments;
  if (segments.size() > 1) {
    std::ranges::sort(segments);
    segments.erase(std::ranges::unique(segments).begin(), segments.end());
  }
}

void GraphicalWire::removeSegment(const GraphicalWireSegment* segment)
{
  const auto b = segments.begin();
  const auto e = segments.end();

  // If pointers are not valid then we can safely return. The segment won't be there
  // anyway
  if (!b.base() || !e.base())
    return;

  const auto pos = std::find(b, e, segment);

  if (pos != e) {
    prepareGeometryChange();
    segments.erase(pos);
  }
}

void GraphicalWire::setBusSize(const unsigned int size)
{
  this->bus.setSize(size);
  prepareGeometryChange();
}

QRectF GraphicalWire::boundingRect() const
{
  QRectF rect{};

  for (QGraphicsItem* child : childItems())
    rect = rect.united(child->boundingRect());

  return rect;
}

QPainterPath GraphicalWire::shape() const
{
  QPainterPath combinedPath{};

  for (const auto segment : this->segments) {
    assert(segment->parentItem() == this);
    combinedPath.addPath(segment->mapToParent(segment->shape()).simplified());
    // combinedPath.connectPath(segment->shape());
  }

  return combinedPath;
}

void GraphicalWire::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget)
{
  // Draw junctions
  painter->setPen(QPen(Qt::blue, 3));
  painter->setBrush(Qt::black);

  for (auto junction : getJunctions())
    painter->drawEllipse(junction, 3, 3);

  // Draw selection box
  if (isSelected()) {
    painter->setBrush(Qt::transparent);
    painter->setPen(QPen(Qt::black, 3, Qt::DotLine));
    painter->drawRect(this->boundingRect());
  }
}

void GraphicalWire::clearBusState()
{
  for (int i = 0; i < this->bus.size(); i++)
    if (bus[i])
      bus[i]->forceSetCurrentState(State::ERROR);
}
GraphicalWireSegment* GraphicalWire::segmentAtPoint(const QPointF point) const
{
  for (const auto segment : segments) {
    const QPointF segmentPoint = segment->mapFromScene(point);
    if (segment->isPointOnPath(segmentPoint)) {
      return segment;
    }
  }

  return nullptr;
}

std::vector<QPointF> GraphicalWire::getJunctions() const
{
  std::vector<QPointF> junctions = {};

  const auto b = this->segments.begin();
  const auto e = this->segments.end();

  // Unordered pairs of segments
  for (auto first = b; first != e; ++first) {
    for (auto second = first + 1; second != e; ++second) {
      const QPointF firstPoint = (*second)->firstPoint();
      const QPointF lastPoint  = (*second)->lastPoint();

      const bool firstPointIntersects = (*first)->isPointOnPath(firstPoint);
      const bool lastPointIntersects  = (*first)->isPointOnPath(lastPoint);

      const bool intersects = firstPointIntersects || lastPointIntersects;
      const bool isSameWire =
          (*first)->firstPoint() == lastPoint || (*first)->lastPoint() == firstPoint;

      if (intersects && !isSameWire) {
        junctions.push_back(firstPointIntersects ? firstPoint : lastPoint);
      } else if (isSameWire) {
        // TODO: MERGE WIRES
      }
    }
  }

  return junctions;
}

std::vector<QPointF> GraphicalWire::getVertices() const
{
  const auto junctions = getJunctions();

  const auto e = junctions.end();

  std::vector<QPointF> vertices = {};

  for (const auto segment : segments) {
    if (std::ranges::find(junctions, segment->lastPoint()) == e)
      vertices.push_back(segment->lastPoint());
    if (std::ranges::find(junctions, segment->firstPoint()) == e)
      vertices.push_back(segment->firstPoint());
  }

  assert(!vertices.empty());
  return vertices;
}

GraphicalWire::~GraphicalWire()
{
  // Enforce the calling of the segment destructor before the wire itself gets destructed
  for (const auto& segment : this->segments) {
    delete segment;
  }
}

GraphicalWireSegment::GraphicalWireSegment(QPointF firstPoint, QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  setFlag(QGraphicsItem::ItemIsSelectable, false);

  points.push_back(firstPoint);
  updatePath();
}

void GraphicalWireSegment::setGraphicalWire(GraphicalWire* graphicalWire)
{
  setParentItem(graphicalWire);

  // The flag is deleted by QGraphicsItem::setParentItem()
  graphicalWire->setFlag(QGraphicsItem::ItemIsSelectable);
  graphicalWire->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

  graphicalWire->addSegment(this);
  this->graphicalWire = graphicalWire;
}

void GraphicalWireSegment::setShowPoints(const std::vector<QPointF>& points)
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

  const auto showStroke = stroker.createStroke(showPath);

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
  for (unsigned int i = 1; i < this->points.size(); i++)
    path.lineTo(this->points[i]);

  // Draw showpoints
  if (!this->showPoints.empty()) {
    showPath.moveTo(this->lastPoint());

    for (auto showPoint : this->showPoints) {
      showPath.lineTo(showPoint);
    }
  }
}

void GraphicalWireSegment::paint(QPainter*                       painter,
                                 const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  const int    size  = (this->graphicalWire) ? this->graphicalWire->getBus().size() : 1;
  const QColor color = (size > 1) ? Qt::darkGreen : Qt::blue;

  painter->setPen(QPen(color, 3));
  painter->drawPath(path);

  // Draw the showPath in red
  painter->setPen(QPen(Qt::red, 3));
  painter->drawPath(showPath);

  if (size > 1) {
    painter->setPen(QPen(color, 2.0));
    painter->setFont(QFont("NovaMono", painter->font().pointSize() * 0.8));

    const qreal totalLength = path.length();

    if (totalLength < 2 * interval) {
      return;  // Path is too short to draw any slashes
    }

    int counter = 0;
    // Iterate along the path's length
    for (qreal dist = interval; dist < totalLength; dist += interval) {
      /* 0   1   2   3   4   5   6 *
       * |  [ ]  |       |  [ ]  | */

      const bool drawSlash = counter % 2 == 0;
      const bool drawBox   = (counter - 1) % 4 == 0;

      // Calculate the percentage along the path for the current distance
      qreal percent = path.percentAtLength(dist);
      painter->save();

      // Get the point and angle at that percentage
      QPointF centerPoint = path.pointAtPercent(percent);
      qreal   pathAngle   = path.angleAtPercent(percent);

      // Move the coordinate system's origin to the center of our slash
      painter->translate(centerPoint);

      // Rotate the coordinate system. `angleAtPercent()` is counter-clockwise whilst
      // `rotate()` is clockwise, we use a negative angle to align.
      painter->rotate(-pathAngle);

      if (drawSlash) {
        // Now that the coordinate system is aligned with the path we can draw a simple
        // rotated line.
        painter->rotate(slashAngle);

        constexpr qreal halfLen = slashLength / 2.0;
        painter->drawLine(QPointF(-halfLen, 0), QPointF(halfLen, 0));
      }

      else if (drawBox) {
        // Rotate the coordinate system back to the original position in order not to
        // write the size upside down
        if (pathAngle == 180)
          painter->rotate(180);

        const QRect   box  = QRect(-boxWidth / 2, -boxHeight / 2, boxWidth, boxHeight);
        const QString text = QString("%1").arg(size);

        painter->setBrush(INTERNAL_COLOR);
        painter->drawRoundedRect(box, 5, 5);
        painter->drawText(box, text, QTextOption(Qt::AlignCenter));
      }

      // Restore the painter's state to what it was before the save()
      painter->restore();
      counter++;
    }
  }
}

// FIXME: The path shape behaves like the path is closed even if it's not.

QRectF GraphicalWireSegment::boundingRect() const
{
  return this->path.boundingRect()
      .united(this->showPath.boundingRect())
      .adjusted(-5, -5, 5, 5)
      .adjusted(-boxHeight / 2, -boxHeight / 2, boxHeight / 2, boxHeight / 2);
}

QPainterPath GraphicalWireSegment::shape() const
{
  QPainterPathStroker stroker;
  return stroker.createStroke(this->path);
}

bool GraphicalWireSegment::isPointOnPath(const QPointF point)
{
  // Add a small tolerance for point detection
  constexpr double tolerance = 5.0;

  if (points.empty())
    return false;

  if (points.size() == 1)
    return QLineF(point, points[0]).length() <= tolerance;

  const auto slide_view = points | std::views::slide(2);

  // For each sub-segment
  for (const auto el : slide_view) {  // NOLINT(*-use-anyofallof)
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