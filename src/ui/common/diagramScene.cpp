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

#include "diagramScene.hpp"

DiagramScene::DiagramScene(QObject* parent) : QGraphicsScene(parent)
{
  setInteractionMode(NORMAL_MODE, true);
}

QPointF DiagramScene::snapToGrid(QPointF point)
{
  auto x = round(point.x() / DiagramScene::GRID_SIZE) * DiagramScene::GRID_SIZE;
  auto y = round(point.y() / DiagramScene::GRID_SIZE) * DiagramScene::GRID_SIZE;

  return QPointF(x, y);
}

void DiagramScene::drawBackground(QPainter* painter, const QRectF& rect)
{
  // Draw the grid to help with components alignment

  QPen pen;
  painter->setPen(pen);

  qreal left = int(rect.left()) - (int(rect.left()) % DiagramScene::GRID_SIZE);
  qreal top  = int(rect.top()) - (int(rect.top()) % DiagramScene::GRID_SIZE);

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
  if (getInteractionMode() == mode && !force)
    return;

  // If the new mode is not wire creation we are not creating any wire anymore.
  if (mode != WIRE_CREATION_MODE) {
    clearWireShadow();
  }

  // Let's do the same thing for component placing mode
  if (mode != COMPONENT_PLACING_MODE) {
    clearComponentShadow();
  }

  this->currentInteractionMode = mode;
  emit DiagramScene::modeChanged(mode);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  // TODO: Print preview of component and wire while placing
  switch (currentInteractionMode) {
    case NORMAL_MODE:
      break;
    case PAN_MODE:
      break;
    case COMPONENT_PLACING_MODE:
      break;
    case WIRE_CREATION_MODE: {
      // Let's wait the user to start drawing the wire
      if (!wireSegmentToBeDrawn)
        break;

      /* Calculate path to the cursor */

      const QPointF cursorPos       = DiagramScene::snapToGrid(mouseEvent->scenePos());
      const QPointF lp              = wireSegmentToBeDrawn->lastPoint();
      const QPointF displacement    = cursorPos - lp;
      const QPointF intermediatePos = (displacement.x() >= displacement.y())
                                          ? QPointF(cursorPos.x(), lp.y())
                                          : QPointF(lp.x(), cursorPos.y());

      std::vector<QPointF> pointsToBeAdded = {intermediatePos, cursorPos};

      // Remove duplicates (if cursorpos is reachable moving only in one direction)
      pointsToBeAdded.erase(unique(pointsToBeAdded.begin(), pointsToBeAdded.end()),
                            pointsToBeAdded.end());

      wireSegmentToBeDrawn->setShowPoints(pointsToBeAdded);
    }
    case SIMULATION_MODE:
      break;
    default:
      assert(false);
  }
  QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  switch (currentInteractionMode) {
    case NORMAL_MODE:
      break;
    case COMPONENT_PLACING_MODE:
      // TODO: Print preview of component while placing
      break;
    case PAN_MODE:
      break;
    case WIRE_CREATION_MODE: {
      if (!wireSegmentToBeDrawn) {
        // Let's start drawing the wire!
        const QPointF cursorPos  = mouseEvent->scenePos();
        const QPointF firstPoint = DiagramScene::snapToGrid(cursorPos);
        wireSegmentToBeDrawn     = new GraphicalWireSegment(firstPoint);
        addItem(wireSegmentToBeDrawn);
      } else {
        // FIXME: When a wire intersects *another* wire it should create a junction.
        //        When a wiresegment has two endpoints it should be associated to a wire.

        wireSegmentToBeDrawn->addPoints();
      }
      break;
    }
    case SIMULATION_MODE:
      break;
    default:
      assert(false);
  }
  QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::keyPressEvent(QKeyEvent* event)
{
  switch (event->key()) {
    case Qt::Key_Escape: {
      setInteractionMode(NORMAL_MODE);
    }
  }
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
