#include "diagramView.hpp"

DiagramView::DiagramView(QWidget* parent) : QGraphicsView(parent) {
  setMouseTracking(true);
  //setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
}

void DiagramView::wheelEvent(QWheelEvent *event) {
  const bool zoomDirection = event->angleDelta().y() > 0;

  zoom(zoomDirection);
  centerOn(mapToScene(event->position().toPoint()));
  event->accept();
}

void DiagramView::zoom(bool dir) {

  const int sign = dir ? 1 : -1;
  zoom(zoomLevel + sign * 20);
  
}

void DiagramView::zoom(int level) {

  if      (level > MAX_ZOOM_LEVEL) zoomLevel = MAX_ZOOM_LEVEL;
  else if (level < MIN_ZOOM_LEVEL) zoomLevel = MIN_ZOOM_LEVEL;
  else                             zoomLevel = level;

  updateZoom();
}

void DiagramView::updateZoom() {
  resetTransform();

  const float FACTOR = (float) zoomLevel / 100;
  
  scale(FACTOR, FACTOR);
}

int DiagramView::getZoomLevel() {
  return zoomLevel;
}
