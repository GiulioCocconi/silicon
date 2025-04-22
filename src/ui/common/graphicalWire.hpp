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

#pragma once

#include <cassert>
#include <ranges>
#include <vector>

#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QPoint>

#include <core/wire.hpp>

class GraphicalWire;

class GraphicalWireJunction : public QGraphicsItem {
public:
  GraphicalWireJunction(QPoint point, QGraphicsItem* parent);
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;

private:
  QRectF boundingRect() const override;

  static const int RADIUS = 5;

  QPoint point;
};

class GraphicalWireSegment : public QGraphicsItem {
public:
  GraphicalWireSegment(QPointF firstPoint, QGraphicsItem* parent = nullptr);
  void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                     QWidget* widget) override;
  QPainterPath shape() const override;

  bool isCompleted() { return false; };
  bool isPointOnPath(const QPointF point);

  void addPoints();

  void                 setShowPoints(std::vector<QPointF> showPoints);
  std::vector<QPointF> getShowPoints() { return showPoints; }

  QPointF lastPoint() { return points[points.size() - 1]; }

private:
  QPainterPath path;

  std::vector<QPointF> points;
  std::vector<QPointF> showPoints;
  GraphicalWire*       graphicalWire = nullptr;

  QRectF boundingRect() const override;

  void updatePath();
};

class GraphicalWire : public QGraphicsItem {
public:
  GraphicalWire(QGraphicsItem* parent = nullptr);
  GraphicalWire(std::vector<GraphicalWireSegment> segments,
                QGraphicsItem*                    parent = nullptr);

  void setBus(Bus bus);
  Bus  getBus();

private:
  Bus bus;
};
