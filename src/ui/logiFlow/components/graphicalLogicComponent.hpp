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

#include <string>
#include <utility>
#include <vector>

#include <QGraphicsItem>
#include <QPainter>
#include <QPoint>
#include <QRect>

#include <core/component.hpp>
#include <core/wire.hpp>

#include <ui/common/diagramScene.hpp>
#include <ui/common/graphicalComponent.hpp>

struct Port {
  QPoint             position;
  Bus                associatedBus;
  QGraphicsLineItem* line;
  std::string        name;
};

class GraphicalLogicComponent : public GraphicalComponent {
  Q_OBJECT
protected:
  std::vector<Port> inputPorts;
  std::vector<Port> outputPorts;
  Component_ptr     associatedComponent;

  bool isEditable;

  bool isColliding;

  void setPortLine(Port& port);

public:
  GraphicalLogicComponent(const Component_ptr component, QGraphicsItem* shape,
                          QGraphicsItem* parent = nullptr);

  void setPorts(const std::vector<std::pair<std::string, QPoint>> busToPortInputs,
                const std::vector<std::pair<std::string, QPoint>> busToPortOutputs);
};
