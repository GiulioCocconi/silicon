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

#pragma once

#include <cassert>
#include <ranges>
#include <vector>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QPoint>
#include <QRect>

#include <ui/common/enums.hpp>

#include <core/wire.hpp>

class GraphicalWire;

class GraphicalWireSegment : public QGraphicsItem {
public:
  GraphicalWireSegment(QPointF firstPoint, QGraphicsItem* parent = nullptr);
  int type() const override { return SiliconTypes::WIRE_SEGMENT; }

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  QPainterPath shape() const override;

  bool isCompleted() { return false; };
  bool isPointOnPath(const QPointF point);

  void addPoints();

  void                 setShowPoints(const std::vector<QPointF>& showPoints);
  std::vector<QPointF> getShowPoints() { return showPoints; }

  QPointF lastPoint() const { return points[points.size() - 1]; }
  QPointF firstPoint() const { return points[0]; }
  QPointF lastShowPoint() const { return points[showPoints.size() - 1]; }

  bool empty() const { return points.size() == 1; }

  GraphicalWire* getGraphicalWire() const { return graphicalWire; }
  void           setGraphicalWire(GraphicalWire* graphicalWire);

  QRectF boundingRect() const override;

  ~GraphicalWireSegment();

private:
  QPainterPath path;
  QPainterPath showPath;

  std::vector<QPointF> points;
  std::vector<QPointF> showPoints;
  GraphicalWire*       graphicalWire = nullptr;

  void updatePath();

  static constexpr int interval    = 60;
  static constexpr int slashLength = 20;
  static constexpr int slashAngle  = 135;

  static constexpr int boxHeight = 20;
  static constexpr int boxWidth  = interval * 0.6;
};

class GraphicalWire : public QGraphicsItem {
public:
  GraphicalWire(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {};
  GraphicalWire(const std::vector<GraphicalWireSegment*>& segments,
                QGraphicsItem*                            parent = nullptr);

  int type() const override { return SiliconTypes::WIRE; }

  void addSegment(GraphicalWireSegment* segment);
  void removeSegment(const GraphicalWireSegment* segment);

  void setBus(Bus bus) { this->bus = bus; }
  void setBusSize(const unsigned int size);

  Bus getBus() const { return bus; }

  void clearBusState();

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

  [[nodiscard]] GraphicalWireSegment* segmentAtPoint(QPointF point) const;
  [[nodiscard]] std::vector<QPointF>  getJunctions() const;
  [[nodiscard]] std::vector<QPointF>  getVertices() const;
  ~GraphicalWire();

  QPainterPath shape() const override;

private:
  Bus                                bus;
  std::vector<GraphicalWireSegment*> segments;

  QRectF boundingRect() const override;
};
