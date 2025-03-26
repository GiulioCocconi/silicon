#pragma once
#include <QGraphicsView>
#include <QWheelEvent>

#include <ui/components/graphicalComponent.hpp>
#include <ui/components/graphicalGates.hpp>


class DiagramView : public QGraphicsView {
  Q_OBJECT
public:
  explicit DiagramView(QWidget *parent = nullptr);
  void zoom(bool dir);
  void zoom(int level);
  int getZoomLevel();
protected:
  void wheelEvent(QWheelEvent *event) override;
private:
  static const int MIN_ZOOM_LEVEL =  60;
  static const int MAX_ZOOM_LEVEL =  200;
  int zoomLevel = 100;
  void updateZoom();
};
