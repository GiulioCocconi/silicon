/*
 Copyright (c) 2025. Giulio Cocconi

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
  this->collidingStatus = CollidingStatus::NOT_COLLIDING;

  setFlag(QGraphicsItem::ItemSendsGeometryChanges);

  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::AllButtons);

  setItemShape(shape);
}

void GraphicalComponent::setItemShape(QGraphicsItem* shape)
{
  assert(shape);

  if (this->itemShape) {
    prepareGeometryChange();
    delete this->itemShape;
  }

  this->itemShape = shape;
  this->itemShape->setParentItem(this);
}

QRectF GraphicalComponent::boundingRect() const
{
  const qreal margin = isSelected() ? 0 : 4;  // Pen width is 3
  auto        rect   = boundingRectWithoutMargins();
  rect.adjust(-margin, -margin, margin, margin);

  return rect;
}

QRectF GraphicalComponent::boundingRectWithoutMargins() const
{
  QRectF rect = itemShape->boundingRect();  // Start with shape bounds

  for (QGraphicsItem* child : childItems())
    rect = rect.united(child->boundingRect());

  return rect;
}

void GraphicalComponent::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                               QWidget* widget)
{
  if (isSelected()) {
    auto color = isColliding() ? Qt::red : Qt::black;
    auto style = isColliding() ? Qt::SolidLine : Qt::DotLine;

    painter->setPen(QPen(color, 3, style));
    painter->drawRect(this->boundingRectWithoutMargins());
  }
}

QRectF GraphicalComponent::collisionRect()
{
  QPainterPath componentPath{};
  componentPath.addRect(itemShape->boundingRect());

  for (const auto port : inputPorts)
    componentPath.addRect(port->collisionRect());

  for (const auto port : outputPorts)
    componentPath.addRect(port->collisionRect());

  return componentPath.boundingRect();
}

QVariant GraphicalComponent::itemChange(GraphicsItemChange change, const QVariant& value)
{
  // TODO: Implement with QGraphicsItem::ItemRotationChange for rotations

  if (!scene())
    return QGraphicsItem::itemChange(change, value);

  if (change == ItemPositionChange) {
    // 'value' is the new proposed position, snapped to the grid.
    auto proposedPos = DiagramScene::snapToGrid(value.toPointF());

    // Collision detection:
    this->collidingStatus = NOT_COLLIDING;

    // Calculate the bounding rectangle at the *new* position in scene coordinates.
    // Use the item's bounding rectangle, offset by the proposed new position.
    const auto newRect = this->boundingRect().translated(proposedPos);

    // Get a list of items that would collide with this item at the new position.
    const auto collidingItems = scene()->items(newRect, Qt::IntersectsItemBoundingRect);

    for (QGraphicsItem* collidingItem : collidingItems) {
      // Skip collision with self or children
      if (collidingItem == this || childItems().contains(collidingItem))
        continue;

      qDebug() << collidingItem->type();
      if (collidingItem->type() >= AND_GATE) {
        this->collidingStatus = COLLIDING_WITH_COMPONENT;
        goto rejectedPos;

      } else if (collidingItem->type() == WIRE) {
        // Check if the wire shape collides with the component shape excluding the port
        // points
        const auto   collidingWire = qgraphicsitem_cast<GraphicalWire*>(collidingItem);
        QPainterPath collisionPath = collidingWire->shape();
        QPainterPath toBeSubtractedWire{};

        for (const auto vertex : collidingWire->getVertices()) {
          toBeSubtractedWire.addEllipse(vertex, 5, 5);
        }

        collisionPath = collisionPath.subtracted(toBeSubtractedWire);
        const bool collidingWithWire =
            collisionPath.intersects(collisionRect().translated(proposedPos));

        if (collidingWithWire) {
          collidingStatus = COLLIDING_WITH_WIRE;
          goto rejectedPos;
        }
      }
    }

    // If there isn't any collision return the proposed position
    return proposedPos;

  rejectedPos:
    assert(collidingStatus != NOT_COLLIDING);
    prepareGeometryChange();
    return pos();  // Return current position, rejecting the change
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
    default: assert(false);
  }
}

void GraphicalComponent::setPorts(
    const std::vector<std::pair<std::string, QPoint>>& busToPortInputs,
    const std::vector<std::pair<std::string, QPoint>>& busToPortOutputs)
{
  for (const auto& [index, pair] : std::views::enumerate(busToPortInputs)) {
    const auto& [name, pos] = pair;

    // The memory is not leaked since the port address is freed by Qt Garbage collector
    // ReSharper disable once CppDFAMemoryLeak
    auto p = new Port(index, pos, name);
    this->inputPorts.push_back(p);
    this->setPortLine(p);
  }

  for (const auto& [index, pair] : std::views::enumerate(busToPortOutputs)) {
    const auto& [name, pos] = pair;

    // ReSharper disable once CppDFAMemoryLeak
    auto p                  = new Port(index, pos, name);
    this->outputPorts.push_back(p);
    this->setPortLine(p);
  }
}

void GraphicalComponent::setPortLine(Port* port)
{
  const auto width  = static_cast<int>(getItemShape()->boundingRect().width());
  const auto height = static_cast<int>(getItemShape()->boundingRect().height());

  const auto portPos = port->getPosition();
  const auto portX   = portPos.x();
  const auto portY   = portPos.y();

  QPoint projectionOnShape{};

  // Left side
  if (portX < 0) {
    projectionOnShape = QPoint(0, portY);
  }
  // Right side
  else if (portX > width)
    projectionOnShape = QPoint(width, portY);

  // Up side
  else if (portY < 0)
    projectionOnShape = QPoint(portX, 0);

  // Down side
  else if (portY > height)
    projectionOnShape = QPoint(portX, height);
  else
    assert(false);

  port->setLine(new QGraphicsLineItem(QLineF(portPos, projectionOnShape), this));
}

Port::Port(unsigned int index, QPoint position, std::string name, QGraphicsItem* parent)
  : QGraphicsItem(parent)
{
  this->index    = index;
  this->position = position;
  this->name     = std::move(name);
}

void Port::setLine(QGraphicsLineItem* line)
{
  this->line = line;
  this->line->setPen(QPen(QBrush(Qt::black), 3));
  setParentItem(line->parentItem());
}

QRectF Port::collisionRect() const
{
  QPainterPath boundingPath{};
  boundingPath.addRect(boundingRect());

  QPainterPath subtract{};
  subtract.addEllipse(position, 5, 5);

  return boundingPath.subtracted(subtract).boundingRect();
}
