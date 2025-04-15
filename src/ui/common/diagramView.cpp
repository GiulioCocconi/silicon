#include "diagramView.hpp"

DiagramView::DiagramView(QWidget* parent) : QGraphicsView(parent) {
  setMouseTracking(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setDragMode(QGraphicsView::RubberBandDrag);
}

void DiagramView::setScene(DiagramScene* scene) {
  connect(scene, &DiagramScene::modeChanged,
	  this,  &DiagramView::modeChanged);
  
  QGraphicsView::setScene(scene);
}

void DiagramView::wheelEvent(QWheelEvent *event) {
  const bool zoomDirection = event->angleDelta().y() > 0;

  if (zoom(zoomDirection))
    centerOn(mapToScene(event->position().toPoint()));
  event->accept();
}

bool DiagramView::zoom(bool dir) {

  const int sign = dir ? 1 : -1;
  return zoom(zoomLevel + sign * 20);

}

bool DiagramView::zoom(int level) {


  if      (level > MAX_ZOOM_LEVEL) level = MAX_ZOOM_LEVEL;
  else if (level < MIN_ZOOM_LEVEL) level = MIN_ZOOM_LEVEL;

  if (level != this->zoomLevel) {
    this->zoomLevel = level;
    updateZoom();
    return true;
  }

  return false;
}

void DiagramView::updateZoom() {
  resetTransform();

  const float FACTOR = (float) zoomLevel / 100;

  scale(FACTOR, FACTOR);
}

int DiagramView::getZoomLevel() {
  return zoomLevel;
}

