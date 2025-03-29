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

#include <vector>
#include <string>
#include <utility>

#include <QPoint>
#include <QRect>
#include <QPainter>
#include <QGraphicsItem>

#include <core/wire.hpp>
#include <core/component.hpp>

struct Port {
  QPoint             position;
  QPoint             realPosition;
  Bus                associatedBus;
  QGraphicsLineItem* line;
  std::string        name;
};

class GraphicalComponent : public QGraphicsItem {
protected:

  std::vector<Port> inputPorts;
  std::vector<Port> outputPorts;
  Component_ptr     associatedComponent;

  QGraphicsItem*     shape; // Without ports

  bool isEditable;

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setPortLine(Port& port);

public:

  GraphicalComponent(const Component_ptr component,
		     QGraphicsItem* shape,
		     QGraphicsItem* parent = nullptr);
  void setPorts(const std::vector<std::pair<std::string, QPoint>> busToPortInputs,
		const std::vector<std::pair<std::string, QPoint>> busToPortOutputs);


};

