#pragma once
#include <QGraphicsView>
#include <QWheelEvent>

#include <ui/common/diagramScene.hpp> // Needed for the grid size

class DiagramView : public QGraphicsView {
  Q_OBJECT
public:
  explicit DiagramView(QWidget *parent = nullptr);
  void setScene(DiagramScene* scene);
  
  bool zoom(bool dir);
  bool zoom(int level);
  int getZoomLevel();

protected:
  void wheelEvent(QWheelEvent *event) override;

public slots:
  void modeChanged(InteractionMode mode);
  
private:
  static const int MIN_ZOOM_LEVEL =  60;
  static const int MAX_ZOOM_LEVEL =  200;
  int zoomLevel = 100;
  void updateZoom();
};
