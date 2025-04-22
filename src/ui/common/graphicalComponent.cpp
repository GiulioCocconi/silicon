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

GraphicalComponent::GraphicalComponent(QGraphicsItem* shape, QGraphicsItem* parent)
  : QGraphicsObject(parent)
{
  this->isEditable = true;

  setFlag(QGraphicsItem::ItemSendsGeometryChanges);

  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::AllButtons);

  assert(shape);
  this->shape = shape;
  this->shape->setParentItem(this);
}

QRectF GraphicalComponent::boundingRect() const
{
  assert(shape);

  const qreal margin = isSelected() ? 0 : 4;  // Pen width is 3
  auto        rect   = boundingRectWithoutMargins();
  rect.adjust(-margin, -margin, margin, margin);

  return rect;
}

QRectF GraphicalComponent::boundingRectWithoutMargins() const
{
  QRectF rect = shape->boundingRect();  // Start with shape bounds

  for (QGraphicsItem* child : childItems())
    rect = rect.united(child->boundingRect());

  return rect;
}

void GraphicalComponent::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget)
{
  if (isSelected()) {
    auto color = isColliding ? Qt::red : Qt::black;
    auto style = isColliding ? Qt::SolidLine : Qt::DotLine;

    painter->setPen(QPen(color, 3, style));
    painter->drawRect(this->boundingRectWithoutMargins());
  }
}

QVariant GraphicalComponent::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionChange && scene()) {
    // 'value' is the new proposed position, snapped to the grid.
    auto proposedPos = DiagramScene::snapToGrid(value.toPointF());

    // Collision detection:
    this->isColliding = false;

    // Calculate the bounding rectangle at the *new* position in scene coordinates.
    // Use the item's bounding rectangle, offset by the proposed new position.
    auto newRect = this->boundingRect().translated(proposedPos);

    // Get a list of items that would collide with this item at the new position.
    // Use the bounding rect for collision check.
    auto collidingItems = scene()->items(newRect, Qt::IntersectsItemBoundingRect);
    // TODO: Use Qt::IntersectsItemShape for more precise collision based on shape().

    for (QGraphicsItem* collidingItem : collidingItems) {
      // Skip collision with self or children
      if (collidingItem == this || childItems().contains(collidingItem))
        continue;

      this->isColliding = true;
      this->update();
      return pos();  // Return current position, rejecting the change
    }

    // If there isn't any collision return the proposed position
    return proposedPos;
  }

  // For all other changes call the base class implementation
  return QGraphicsItem::itemChange(change, value);
}

void GraphicalComponent::modeChanged(InteractionMode mode)
{
  switch (mode) {
    case DiagramScene::NORMAL_MODE:
      setFlag(QGraphicsItem::ItemIsMovable);
      setFlag(QGraphicsItem::ItemIsFocusable);
      setFlag(QGraphicsItem::ItemIsSelectable);
      break;
    case DiagramScene::WIRE_CREATION_MODE:
    case DiagramScene::COMPONENT_PLACING_MODE:
    case DiagramScene::SIMULATION_MODE:
    case DiagramScene::PAN_MODE:
      setFlag(QGraphicsItem::ItemIsSelectable, false);
      setFlag(QGraphicsItem::ItemIsMovable, false);
      setFlag(QGraphicsItem::ItemIsFocusable, false);
      break;
    default:
      assert(false);
  }
}
