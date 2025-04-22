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

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
#include <QPoint>
#include <QRect>

#include <core/component.hpp>
#include <ui/common/diagramScene.hpp>

enum CollidingStatus {
  NOT_COLLIDING,
  COLLIDING_WITH_COMPONENT,
  COLLIDING_WITH_PORT,
  COLLIDING_WITH_WIRE
};

class GraphicalComponent : public QGraphicsObject {
  Q_OBJECT
protected:
  QGraphicsItem* shape;

  QRectF boundingRect() const override;
  QRectF boundingRectWithoutMargins() const;

  void     paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

  CollidingStatus collidingStatus = CollidingStatus::NOT_COLLIDING;

public slots:
  void modeChanged(InteractionMode mode);

public:
  GraphicalComponent(QGraphicsItem* shape, QGraphicsItem* parent = nullptr);

  bool isColliding() { return collidingStatus != NOT_COLLIDING; }
  
};
