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

#include "graphicalComponent.hpp"



GraphicalComponent::GraphicalComponent(const Component_ptr component,
				       QGraphicsItem* shape,
				       QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  
  this->isEditable = true;
  
  setFlags(QGraphicsItem::ItemIsMovable
	   | QGraphicsItem::ItemIsSelectable
	   | QGraphicsItem::ItemIsFocusable
	   | QGraphicsItem::ItemSendsGeometryChanges);

  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::AllButtons);


  assert(shape);
  this->shape = shape;
  this->shape->setParentItem(this);

  qDebug() << "Bottom left: "  << this->shape->boundingRect().bottomLeft();
  qDebug() << "Bottom right: " << this->shape->boundingRect().bottomRight();
  qDebug() << "Center: "       << this->shape->boundingRect().center();
  
  this->associatedComponent = component;
}

void GraphicalComponent::setPorts(const std::vector<std::pair<std::string, QPoint>> busToPortInputs,
				  const std::vector<std::pair<std::string, QPoint>> busToPortOutputs)
{
  
  std::vector<Bus> componentInputs  = associatedComponent.lock()->getInputs();
  std::vector<Bus> componentOutputs = associatedComponent.lock()->getOutputs();
  
  assert(componentInputs.size()  == busToPortInputs.size());
  assert(componentOutputs.size() == busToPortOutputs.size());
  
  for (int i = 0; i < busToPortInputs.size(); i++) {
    Port p {};
    p.name          = busToPortInputs[i].first;
    p.position      = busToPortInputs[i].second;
    p.associatedBus = componentInputs[i];
    p.boundingRect  = QRect(p.position - QPoint(5, 5), QSize(5, 5)); 
    this->inputPorts.push_back(p);
  }

  for (int i = 0; i < busToPortOutputs.size(); i++) {
    Port p {};
    p.name          = busToPortOutputs[i].first;
    p.position      = busToPortOutputs[i].second;
    p.associatedBus = componentOutputs[i];
    p.boundingRect  = QRect(p.position - QPoint(5, 5), QSize(10, 10)); 
    
    this->outputPorts.push_back(p);
  }  
}

QRectF GraphicalComponent::boundingRect() const {

  assert(shape);
  QRectF rect = shape->boundingRect(); // Start with shape bounds

  // Expand to include input ports
  for (const auto &port : inputPorts) {
    rect = rect.united(port.boundingRect);
  }

  // Expand to include output ports
  for (const auto &port : outputPorts) {
    rect = rect.united(port.boundingRect);
  }

  return rect;
}


void GraphicalComponent::paint(QPainter *painter,
			       const QStyleOptionGraphicsItem *option,
			       QWidget *widget)
{
  // Paint the ports
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(Qt::black);
  painter->setBrush(Qt::blue); // Or a different color for input/output

  //Draw Input Ports
  for (const auto &port : inputPorts) {
    painter->drawRect(port.boundingRect);
  }

  //Draw Output Ports
  painter->setBrush(Qt::red);
  for (const auto &port : outputPorts) {
    painter->drawRect(port.boundingRect);
  }

  // The shape is then painted cause it is its children!
  
}
