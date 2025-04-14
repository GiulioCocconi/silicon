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

#include <QPoint>
#include <QRect>
#include <QPainter>
#include <QGraphicsItem>

#include <core/component.hpp>
#include <ui/common/diagramScene.hpp>

class GraphicalComponent : public QGraphicsObject {
  Q_OBJECT
protected:

  QGraphicsItem*     shape; // Without ports

  bool isEditable;

  bool isColliding;

  QRectF boundingRect() const override;
  QRectF boundingRectWithoutMargins() const;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public:

  GraphicalComponent(QGraphicsItem* shape,
		     QGraphicsItem* parent = nullptr);

};

