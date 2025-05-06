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

#include "diagramScene.hpp"
#include "ui/common/enums.hpp"
#include "ui/common/graphicalWire.hpp"
#include "ui/logiFlow/components/graphicalIO.hpp"

DiagramScene::DiagramScene(QObject* parent) : QGraphicsScene(parent)
{
  setInteractionMode(NORMAL_MODE, true);

  csb = new ComponentSearchBox();
  csb->setParent(this);
  connect(csb, &ComponentSearchBox::requestHide, this, &DiagramScene::hideCSB);
}

QPointF DiagramScene::snapToGrid(const QPointF point)
{
  auto x = round(point.x() / DiagramScene::GRID_SIZE) * DiagramScene::GRID_SIZE;
  auto y = round(point.y() / DiagramScene::GRID_SIZE) * DiagramScene::GRID_SIZE;

  return {x, y};
}

void DiagramScene::drawBackground(QPainter* painter, const QRectF& rect)
{
  // Draw the grid to help with components alignment

  QPen pen;
  painter->setPen(pen);

  const qreal left = int(rect.left()) - (int(rect.left()) % DiagramScene::GRID_SIZE);
  const qreal top  = int(rect.top()) - (int(rect.top()) % DiagramScene::GRID_SIZE);

  QVector<QPointF> points;
  for (qreal x = left; x < rect.right(); x += DiagramScene::GRID_SIZE) {
    for (qreal y = top; y < rect.bottom(); y += DiagramScene::GRID_SIZE) {
      points.append(QPointF(x, y));
    }
  }
  painter->drawPoints(points.data(), points.size());
}

void DiagramScene::setInteractionMode(InteractionMode mode)
{
  setInteractionMode(mode, false);
}

void DiagramScene::setInteractionMode(InteractionMode mode, bool force)
{
  if (!force)
    assert(views().size() == 1);
  const auto currentMode = getInteractionMode();
  if (currentMode == mode && !force)
    return;

  if (wireSegmentToBeDrawn && mode != WIRE_CREATION_MODE) {
    // Remove the wireSegment if it's invisible
    if (wireSegmentToBeDrawn->empty()) {
      removeItem(wireSegmentToBeDrawn);
      delete wireSegmentToBeDrawn;
      wireSegmentToBeDrawn = nullptr;
    } else if (!wireSegmentToBeDrawn->getGraphicalWire()) {
      // Create wire for orphan segments :(
      qDebug() << "Creating wire";

      // Create the bus
      const auto b = Bus(1);
      auto*      w = new GraphicalWire();
      w->setBus(b);

      wireSegmentToBeDrawn->setGraphicalWire(w);
      addItem(w);
    }
    clearWireShadow();
  }

  if (currentMode == COMPONENT_PLACING_MODE) {
    hideCSB();
    if (componentToBeDrawn)
      clearComponentShadow();

  } else if (mode == COMPONENT_PLACING_MODE) {

    // We need to show the CSB

    // ALGORITHM: If the cursor is inside the view then try to place the component.
    //            If the it's outside or the component boundingRect is colliding then
    //            go to component placing mode and place it manually.
    //            When the component is placed then go to component placing mode and
    //            repeat the placing of the same component until ESC is pressed
    //            (NORMAL_MODE)

    const QPoint globalCursorPos = QCursor::pos();


    const auto view = this->views()[0];

    // The default position is the center of the view
    QPoint posForCSB = view->viewport()->rect().center();

    // Get cursor pos within view
    const QPoint viewCursorPos = view->mapFromGlobal(globalCursorPos);

    const bool isCursorInsideView =
        view->viewport()->rect().contains(viewCursorPos);

    if (isCursorInsideView) posForCSB = viewCursorPos;

    showCSB(view->mapToScene(posForCSB));

  }

  if (mode == SIMULATION_MODE || currentMode == SIMULATION_MODE) {
    // If we are goint to simulation mode then calculate the wires
    if (mode == SIMULATION_MODE)
      calculateWiresForComponents();

    // RESTORE INPUTS TO NEUTRAL STATE

    // TODO: Make a parent IO class with virtual reset method

    const auto inputComponents =
        items() | std::views::filter([](auto item) {
          return item->type() == SiliconTypes::SINGLE_INPUT;
        })
        | std::views::transform(
            [](auto item) { return qgraphicsitem_cast<GraphicalInputSingle*>(item); })
        | std::ranges::to<std::vector>();

    for (const auto inputComponent : inputComponents) {
      inputComponent->setState(LOW);
    }

    // If we are exiting SIMULATION_MODE then restore outputs as well
    if (currentMode == SIMULATION_MODE) {
      const auto outputComponents =
          items() | std::views::filter([](auto item) {
            return item->type() == SiliconTypes::SINGLE_OUTPUT;
          })
          | std::views::transform(
              [](auto item) { return qgraphicsitem_cast<GraphicalOutputSingle*>(item); })
          | std::ranges::to<std::vector>();

      for (const auto outputComponent : outputComponents) {
        outputComponent->setState(LOW);
      }
    }
  }

  this->currentInteractionMode = mode;
  emit DiagramScene::modeChanged(mode);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  const QPointF cursorPos = DiagramScene::snapToGrid(mouseEvent->scenePos());

  // TODO: Print preview of component and wire while placing
  switch (currentInteractionMode) {
    case NORMAL_MODE: break;
    case PAN_MODE: break;
    case COMPONENT_PLACING_MODE: break;
    case WIRE_CREATION_MODE: {
      // Let's wait the user to start drawing the wire
      if (!wireSegmentToBeDrawn)
        break;

      /* Calculate path to the cursor */

      const QPointF lp              = wireSegmentToBeDrawn->lastPoint();
      const QPointF displacement    = cursorPos - lp;
      const QPointF intermediatePos = (displacement.x() >= displacement.y())
                                          ? QPointF(cursorPos.x(), lp.y())
                                          : QPointF(lp.x(), cursorPos.y());

      std::vector<QPointF> pointsToBeAdded = {intermediatePos, cursorPos};

      // Remove duplicates (if cursorPos is reachable moving only in one direction)
      pointsToBeAdded.erase(std::ranges::unique(pointsToBeAdded).begin(),
                            pointsToBeAdded.end());

      wireSegmentToBeDrawn->setShowPoints(pointsToBeAdded);
    }
    case SIMULATION_MODE: break;
    default: assert(false);
  }
  QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  const QPointF cursorPos = mouseEvent->scenePos();

  switch (currentInteractionMode) {
    case NORMAL_MODE: break;
    case COMPONENT_PLACING_MODE:
      // TODO: Print preview of component while placing
      break;
    case PAN_MODE: break;
    case WIRE_CREATION_MODE: {
      // TODO: Check for collision
      if (!wireSegmentToBeDrawn) {
        // Let's start drawing the wire!
        const QPointF firstPoint = DiagramScene::snapToGrid(cursorPos);
        wireSegmentToBeDrawn     = new GraphicalWireSegment(firstPoint);
        addItem(wireSegmentToBeDrawn);
        manageJunctionCreation(cursorPos);
      } else {
        wireSegmentToBeDrawn->addPoints();

        // If the point to be created is a junction then we have finished drawing
        // the segment

        if (manageJunctionCreation(cursorPos)) {
          setInteractionMode(NORMAL_MODE);
          return;
        }

        // TODO: check for ports
      }
      break;
    }
    case SIMULATION_MODE: {
      auto itemsAtPos = items(cursorPos);

      for (auto item : itemsAtPos) {
        if (item && item->type() == SiliconTypes::SINGLE_INPUT) {
          auto* input = qgraphicsitem_cast<GraphicalInputSingle*>(item);
          input->toggle();
        }
      }
      break;
    }
    default: assert(false);
  }
  QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::keyPressEvent(QKeyEvent* event)
{
  switch (event->key()) {
    case Qt::Key_Escape: {
      setInteractionMode(NORMAL_MODE);
      break;
    }
    default: break;
  }
  QGraphicsScene::keyPressEvent(event);
}

void DiagramScene::clearWireShadow()
{
  if (!wireSegmentToBeDrawn)
    return;

  wireSegmentToBeDrawn->setShowPoints({});
  wireSegmentToBeDrawn = nullptr;
}

void DiagramScene::clearComponentShadow()
{
  if (!componentToBeDrawn)
    return;

  componentToBeDrawn = nullptr;
}


void DiagramScene::showCSB(const QPointF pos)
{
  csb->clear();
  csb->setPos(pos);
  addItem(csb);
  csb->focus();
}

void DiagramScene::hideCSB()
{
  removeItem(csb);
}

void DiagramScene::calculateWiresForComponents() const
{
  const auto wires = items()
                     | std::views::filter([](auto item) { return item->type() == WIRE; })
                     | std::views::transform([](auto item) {
                         return qgraphicsitem_cast<GraphicalWire*>(item);
                       })
                     | std::ranges::to<std::vector>();

  // Set wires to initial state
  for (GraphicalWire* wire : wires) {
    wire->clearBusState();
  }

  auto components =
      items() | std::views::filter([](auto item) { return item->type() >= COMPONENT; })
      | std::views::transform(
          [](auto item) { return qgraphicsitem_cast<GraphicalLogicComponent*>(item); })
      | std::ranges::to<std::vector>();

  for (const GraphicalLogicComponent* component : components) {
    assert(component);

    // Disconnect the component from all wires (TODO: Make more efficient)
    component->getComponent()->clearWires();

    // Check for wire collision
    auto collidingWires =
        collidingItems(component)
        | std::views::filter([](auto el) { return el->type() == SiliconTypes::WIRE; })
        | std::views::transform(
            [](auto el) { return qgraphicsitem_cast<GraphicalWire*>(el); })
        | std::ranges::to<std::vector>();

    // Find the GraphicalWires colliding with port
    for (const GraphicalWire* wire : collidingWires) {
      const auto vertices = wire->getVertices();

      for (const auto [index, p] : std::views::enumerate(component->getInputPorts())) {
        const auto portPositionInScene = component->mapToScene(p->getPosition());
        const auto findResult          = std::ranges::find(vertices, portPositionInScene);
        if (findResult != vertices.end()) {
          component->getComponent()->setInput(index, wire->getBus());
        }
      }

      for (const auto [index, p] : std::views::enumerate(component->getOutputPorts())) {
        const auto portPositionInScene = component->mapToScene(p->getPosition());
        const auto findResult          = std::ranges::find(vertices, portPositionInScene);
        if (findResult != vertices.end()) {
          component->getComponent()->setOutput(index, wire->getBus());
        }
      }
    }
  }
}

bool DiagramScene::manageJunctionCreation(const QPointF cursorPos) const
{
  // Using Qt::IntersectsItemBoundingRect cause we don't care about the shape of the wire
  for (const auto item : items(cursorPos, Qt::IntersectsItemBoundingRect)) {
    if (item->type() == WIRE) {
      const auto wire = qgraphicsitem_cast<GraphicalWire*>(item);
      if (wire->segmentAtPoint(cursorPos)) {
        wire->addSegment(wireSegmentToBeDrawn);
        return true;
      }
    }
  }
  return false;
}

DiagramScene::~DiagramScene()
{
  // Clean up any remaining wire segment being drawn
  if (wireSegmentToBeDrawn) {
    removeItem(wireSegmentToBeDrawn);
    delete wireSegmentToBeDrawn;
    wireSegmentToBeDrawn = nullptr;
  }

  // Clean up any component being drawn
  if (componentToBeDrawn) {
    removeItem(componentToBeDrawn);
    delete componentToBeDrawn;
    componentToBeDrawn = nullptr;
  }
}

