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
    this->inputPorts.push_back(p);
    this->setPortLine(p);
  }

  for (int i = 0; i < busToPortOutputs.size(); i++) {
    Port p {};
    p.name          = busToPortOutputs[i].first;
    p.position      = busToPortOutputs[i].second;
    p.associatedBus = componentOutputs[i];
    this->outputPorts.push_back(p);
    this->setPortLine(p);
  }
}

QRectF GraphicalComponent::boundingRect() const {

  assert(shape);
  QRectF rect = shape->boundingRect(); // Start with shape bounds

  for (QGraphicsItem *child : childItems())
    rect = rect.united(child->boundingRect());

  return rect;
}


void GraphicalComponent::paint(QPainter *painter,
			       const QStyleOptionGraphicsItem *option,
			       QWidget *widget)
{

}

void GraphicalComponent::setPortLine(Port& port) {
  // The port must be inside the shape
  assert(shape->boundingRect().contains(port.position));

  // The port mustn't be in the diagonal
  assert(port.position.x() != port.position.y());

  const auto width  = shape->boundingRect().width();
  const auto height = shape->boundingRect().height();


  qDebug() << width;
  qDebug() << height;

  const auto length = 10;

  // Left side
  if (port.position.x() == 0) {
    port.realPosition = QPoint(-length, port.position.y());
  }
  // Right side
  else if (port.position.x() == width)
    port.realPosition = QPoint(width + length, port.position.y());

  // Up side
  else if (port.position.y() == 0)
    port.realPosition = QPoint(port.position.x(), -length);

  // Down side
  else if (port.position.y() == height)
    port.realPosition = QPoint(port.position.x(), height + length);
  else
    assert(false);


  port.line = new QGraphicsLineItem(QLineF(port.realPosition, port.position),
				    this);
  port.line->setPen(QPen(QBrush(Qt::black), 3));



}
