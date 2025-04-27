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

#include "graphicalLogicComponent.hpp"

GraphicalLogicComponent::GraphicalLogicComponent(const Component_ptr component,
                                                 QGraphicsItem*      shape,
                                                 QGraphicsItem*      parent)
  : GraphicalComponent(shape, parent)
{
  this->associatedComponent = component;
}

void GraphicalLogicComponent::setPorts(
    const std::vector<std::pair<std::string, QPoint>> busToPortInputs,
    const std::vector<std::pair<std::string, QPoint>> busToPortOutputs)
{
  if (associatedComponent) {
    std::vector<Bus> componentInputs  = associatedComponent->getInputs();
    std::vector<Bus> componentOutputs = associatedComponent->getOutputs();

    assert(componentInputs.size() == busToPortInputs.size());
    assert(componentOutputs.size() == busToPortOutputs.size());
  }

  for (int i = 0; i < busToPortInputs.size(); i++) {
    Port p{};
    p.name     = busToPortInputs[i].first;
    p.position = busToPortInputs[i].second;
    // p.associatedBus = componentInputs[i];
    this->inputPorts.push_back(p);
    this->setPortLine(p);
  }

  for (int i = 0; i < busToPortOutputs.size(); i++) {
    Port p{};
    p.name     = busToPortOutputs[i].first;
    p.position = busToPortOutputs[i].second;
    // p.associatedBus = componentOutputs[i];
    this->outputPorts.push_back(p);
    this->setPortLine(p);
  }
}

void GraphicalLogicComponent::setPortLine(Port& port)
{
  const auto width  = getShape()->boundingRect().width();
  const auto height = getShape()->boundingRect().height();

  auto projectionOnShape = QPoint();

  // Left side
  if (port.position.x() < 0) {
    projectionOnShape = QPoint(0, port.position.y());
  }
  // Right side
  else if (port.position.x() > width)
    projectionOnShape = QPoint(width, port.position.y());

  // Up side
  else if (port.position.y() < 0)
    projectionOnShape = QPoint(port.position.x(), 0);

  // Down side
  else if (port.position.y() > height)
    projectionOnShape = QPoint(port.position.x(), height);
  else
    assert(false);

  port.line = new QGraphicsLineItem(QLineF(port.position, projectionOnShape), this);
  port.line->setPen(QPen(QBrush(Qt::black), 3));
}
