#pragma once

#include <QColor>
#include <QLabel>
#include <QBrush>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QMainWindow>

#include <ui/diagramScene.hpp>
#include <ui/diagramView.hpp>

#ifndef QT_NO_CONTEXTMENU
    #include <QContextMenuEvent>
#endif

class LogiFlowWindow : public QMainWindow {
  Q_OBJECT

public:
  LogiFlowWindow();

protected:

#ifndef QT_NO_CONTEXTMENU
  void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
  void newFile() {}
  void open() {}
  void save() {}
  void exportImage() {}
  void undo() {}
  void redo() {}
  void cut() {}
  void copy() {}
  void paste() {}
  void about() {}

private:
  void createActions();
  void createMenus();

  QToolBar* toolBar;

  QDockWidget* componentsDock;
  QDockWidget* propertyDock;

  QGraphicsScene* graphicsScene;
  DiagramView*    diagramView;

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* helpMenu;
  QAction* newAct;
  QAction* openAct;
  QAction* saveAct;
  QAction* exportImageAct;
  QAction* exitAct;
  QAction* undoAct;
  QAction* redoAct;
  QAction* cutAct;
  QAction* copyAct;
  QAction* pasteAct;
  QAction* aboutAct;
  QLabel* infoLabel;
};
