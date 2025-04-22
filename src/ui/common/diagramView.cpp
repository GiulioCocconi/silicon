#include "diagramView.hpp"

DiagramView::DiagramView(QWidget* parent) : QGraphicsView(parent)
{
  setMouseTracking(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void DiagramView::setScene(DiagramScene* scene)
{
  connect(scene, &DiagramScene::modeChanged, this, &DiagramView::modeChanged);

  modeChanged(scene->getInteractionMode());

  QGraphicsView::setScene(scene);
}

void DiagramView::wheelEvent(QWheelEvent* event)
{
  const bool zoomDirection = event->angleDelta().y() > 0;

  // If the scene is in Wire Creation mode then the mouse wheel shouldn't do anything
  auto dg = (DiagramScene*)scene();
  if (dg->getInteractionMode() == InteractionMode::WIRE_CREATION_MODE) {
    event->ignore();
    return;
  }

  if (zoom(zoomDirection))
    centerOn(mapToScene(event->position().toPoint()));
  event->accept();
}

bool DiagramView::zoom(bool dir)
{
  const int sign = dir ? 1 : -1;
  return zoom(zoomLevel + sign * 20);
}

bool DiagramView::zoom(int level)
{
  if (level > MAX_ZOOM_LEVEL)
    level = MAX_ZOOM_LEVEL;
  else if (level < MIN_ZOOM_LEVEL)
    level = MIN_ZOOM_LEVEL;

  if (level != this->zoomLevel) {
    this->zoomLevel = level;
    updateZoom();
    return true;
  }

  return false;
}

void DiagramView::updateZoom()
{
  resetTransform();

  const float FACTOR = (float)zoomLevel / 100;

  scale(FACTOR, FACTOR);
}

int DiagramView::getZoomLevel()
{
  return zoomLevel;
}

void DiagramView::modeChanged(InteractionMode mode)
{
  // TODO: Set items flags using this->items()
  
  setCursor(Qt::ArrowCursor);
  setDragMode(QGraphicsView::NoDrag);

 switch (mode) {
    case InteractionMode::NORMAL_MODE:
      setDragMode(QGraphicsView::RubberBandDrag);
      break;
    case InteractionMode::WIRE_CREATION_MODE:
      setCursor(Qt::CrossCursor);
      break;
    case InteractionMode::PAN_MODE:
      setDragMode(QGraphicsView::ScrollHandDrag);
      setCursor(Qt::SizeAllCursor);
      break;
    case InteractionMode::COMPONENT_PLACING_MODE:
    case InteractionMode::SIMULATION_MODE:
      break;
    default:
      assert(false);
  }
}
