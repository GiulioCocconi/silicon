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

  if (isSelected()) {
    auto color = this->isColliding ? Qt::red       : Qt::black;
    auto style = this->isColliding ? Qt::SolidLine : Qt::DotLine;

    painter->setPen(QPen(color, 3, style));
    painter->drawRect(this->boundingRectWithoutMargins());
  }

}

void GraphicalComponent::setPortLine(Port& port) {

  const auto width  = shape->boundingRect().width();
  const auto height = shape->boundingRect().height();

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


  port.line = new QGraphicsLineItem(QLineF(port.position, projectionOnShape),
				    this);
  port.line->setPen(QPen(QBrush(Qt::black), 3));
}



QVariant GraphicalComponent::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (change == ItemPositionChange && scene()) {
    // 'value' is the new proposed position.
    auto proposedPos = value.toPointF();

    // Calculate the bounding rectangle at the *new* position in scene coordinates.
    // Use the item's bounding rectangle, offset by the proposed new position.
    auto newRect = this->boundingRect().translated(proposedPos);

    // Get a list of items that would collide with this item at the new position.
    // Use the bounding rect for collision check.
    auto colliding_items = scene()->items(newRect,
					  Qt::IntersectsItemBoundingRect);
    //TODO: Use Qt::IntersectsItemShape for more precise collision based on shape().

    for (QGraphicsItem *colliding_item : colliding_items) {
      // Skip collision with self or children
      if (colliding_item == this || childItems().contains(colliding_item))
	continue;

      qDebug() << this << "Collision detected with" << colliding_item; // Optional debug
      return pos(); // Return current position, rejecting the change
    }
  }

  // For all other changes (or if there isn't any colliding object),
  // call the base class implementation
  return QGraphicsItem::itemChange(change, value);
};
