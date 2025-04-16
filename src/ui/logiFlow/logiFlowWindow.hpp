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

#include <ui/common/icons.hpp>
#include <ui/common/diagramScene.hpp>
#include <ui/common/diagramView.hpp>

#include <ui/logiFlow/components/graphicalGates.hpp>

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
