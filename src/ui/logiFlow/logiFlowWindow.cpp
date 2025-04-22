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

#include "logiFlowWindow.hpp"
#include "ui/common/diagramScene.hpp"

LogiFlowWindow::LogiFlowWindow()
{
  auto centralWidget = new QWidget;
  setCentralWidget(centralWidget);

  auto layout = new QHBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  centralWidget->setLayout(layout);

  componentsDock = new QDockWidget(this);
  propertyDock   = new QDockWidget(this);

  addDockWidget(Qt::LeftDockWidgetArea, componentsDock);
  addDockWidget(Qt::LeftDockWidgetArea, propertyDock);

  propertyDock->setFeatures(QDockWidget::DockWidgetMovable);
  componentsDock->setFeatures(QDockWidget::DockWidgetMovable);

  propertyDock->setWindowTitle("Properties");
  componentsDock->setWindowTitle("Components");

  splitDockWidget(componentsDock, propertyDock, Qt::Vertical);

  diagramScene = new DiagramScene(this);
  diagramView  = new DiagramView(this);
  diagramView->setScene(diagramScene);

  connect(diagramScene, &DiagramScene::modeChanged, this, &LogiFlowWindow::updateStatus);
  
  updateStatus();

  auto a            = std::make_shared<Wire>(State::HIGH);
  auto o            = std::make_shared<Wire>();
  auto xg           = std::make_shared<XorGate>(std::array<Wire_ptr, 2>{a, a}, o);
  auto ng           = std::make_shared<NotGate>(a, o);
  auto graphicalXor = new GraphicalXor(xg);
  auto graphicalNot = new GraphicalNot(ng);

  graphicalXor->setPos(0, 50);

  diagramScene->addItem(graphicalXor);
  diagramScene->addItem(graphicalNot);

  layout->addWidget(diagramView);

  createActions();
  createMenus();
  createToolBar();

  setWindowTitle(tr("Silicon LogiFlow"));
  setMinimumSize(160, 160);
}

void LogiFlowWindow::createActions()
{
  newAct         = new QAction(Icon("file"), tr("&New"), this);
  openAct        = new QAction(Icon("open"), tr("&Open..."), this);
  saveAct        = new QAction(Icon("save"), tr("&Save"), this);
  exportImageAct = new QAction(Icon("export"), tr("&Export..."), this);
  exitAct        = new QAction(Icon("xmark"), tr("E&xit"), this);
  undoAct        = new QAction(Icon("undo"), tr("&Undo"), this);
  redoAct        = new QAction(Icon("redo"), tr("&Redo"), this);
  cutAct         = new QAction(Icon("cut"), tr("Cu&t"), this);
  copyAct        = new QAction(Icon("copy"), tr("&Copy"), this);
  pasteAct       = new QAction(Icon("paste"), tr("&Paste"), this);
  deleteAct      = new QAction(Icon("delete"), tr("&Delete"), this);
  aboutAct       = new QAction(Icon("info"), tr("&About"), this);

  setNormalModeAct       = new QAction(Icon("mouse-pointer"), "", this);
  setPanModeAct          = new QAction(Icon("pan"), "", this);
  setWireCreationModeAct = new QAction(Icon("link"), "", this);
  setSimulationModeAct   = new QAction(Icon("play"), "", this);

  addComponentAct = new QAction(Icon("plus"), "", this);

  newAct->setShortcuts(QKeySequence::New);
  openAct->setShortcuts(QKeySequence::Open);
  saveAct->setShortcuts(QKeySequence::Save);
  exportImageAct->setShortcuts(QKeySequence::Print);
  exitAct->setShortcuts(QKeySequence::Quit);
  undoAct->setShortcuts(QKeySequence::Undo);
  redoAct->setShortcuts(QKeySequence::Redo);
  cutAct->setShortcuts(QKeySequence::Cut);
  copyAct->setShortcuts(QKeySequence::Copy);
  deleteAct->setShortcuts(QKeySequence::Delete);
  pasteAct->setShortcuts(QKeySequence::Paste);
  

  setWireCreationModeAct->setShortcut(Qt::AltModifier | Qt::Key_W);
  setSimulationModeAct->setShortcut(Qt::AltModifier | Qt::ControlModifier | Qt::Key_S);
  
  addComponentAct->setShortcut(Qt::AltModifier | Qt::Key_A);
  

  newAct->setStatusTip(tr("Create a new file"));
  openAct->setStatusTip(tr("Open an existing logiFlow file"));
  saveAct->setStatusTip(tr("Save the circuit to disk"));
  exportImageAct->setStatusTip(tr("Export the circuit as an image"));
  exitAct->setStatusTip(tr("Exit the application"));
  undoAct->setStatusTip(tr("Undo the last operation"));
  redoAct->setStatusTip(tr("Redo the last operation"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
  deleteAct->setStatusTip(tr("Delete selected components"));
  aboutAct->setStatusTip(tr("Show the application's about box"));

  connect(newAct, &QAction::triggered, this, &LogiFlowWindow::newFile);
  connect(openAct, &QAction::triggered, this, &LogiFlowWindow::open);
  connect(saveAct, &QAction::triggered, this, &LogiFlowWindow::save);
  connect(exportImageAct, &QAction::triggered, this, &LogiFlowWindow::exportImage);
  connect(exitAct, &QAction::triggered, this, &QWidget::close);
  connect(undoAct, &QAction::triggered, this, &LogiFlowWindow::undo);
  connect(redoAct, &QAction::triggered, this, &LogiFlowWindow::redo);
  connect(cutAct, &QAction::triggered, this, &LogiFlowWindow::cut);
  connect(copyAct, &QAction::triggered, this, &LogiFlowWindow::copy);
  connect(pasteAct, &QAction::triggered, this, &LogiFlowWindow::paste);
  connect(deleteAct, &QAction::triggered, this, &LogiFlowWindow::del);
  connect(aboutAct, &QAction::triggered, this, &LogiFlowWindow::about);

  connect(setNormalModeAct, &QAction::triggered, this, &LogiFlowWindow::setNormalMode);
  connect(setPanModeAct, &QAction::triggered, this, &LogiFlowWindow::setPanMode);
  connect(setWireCreationModeAct, &QAction::triggered, this,
          &LogiFlowWindow::setWireCreationMode);
  connect(setSimulationModeAct, &QAction::triggered, this,
          &LogiFlowWindow::setSimulationMode);

  connect(addComponentAct, &QAction::triggered, this, &LogiFlowWindow::addComponent);
}

void LogiFlowWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(exportImageAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(undoAct);
  editMenu->addAction(redoAct);
  editMenu->addSeparator();
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);
  editMenu->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
}

void LogiFlowWindow::createToolBar()
{
  toolBar = new QToolBar(this);
  toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
  toolBar->setFloatable(false);

  toolBar->addAction(newAct);
  toolBar->addAction(openAct);
  toolBar->addAction(saveAct);

  toolBar->addSeparator();

  toolBar->addAction(setNormalModeAct);
  toolBar->addAction(setPanModeAct);
  toolBar->addAction(setWireCreationModeAct);
  toolBar->addAction(setSimulationModeAct);

  toolBar->addSeparator();
  toolBar->addAction(addComponentAct);

  addToolBar(toolBar);
}

#ifndef QT_NO_CONTEXTMENU
void LogiFlowWindow::contextMenuEvent(QContextMenuEvent* event)
{
  QMenu menu(this);
  menu.addAction(cutAct);
  menu.addAction(copyAct);
  menu.addAction(pasteAct);
  menu.addAction(deleteAct);
  menu.exec(event->globalPos());
}
#endif  // QT_NO_CONTEXTMENU

/* ACTIONS IMPLEMENTATION */

void LogiFlowWindow::setNormalMode()
{
  diagramScene->setInteractionMode(InteractionMode::NORMAL_MODE);
}

void LogiFlowWindow::setPanMode()
{
  diagramScene->setInteractionMode(InteractionMode::PAN_MODE);
}

void LogiFlowWindow::setWireCreationMode()
{
  diagramScene->setInteractionMode(InteractionMode::WIRE_CREATION_MODE);
}

void LogiFlowWindow::setSimulationMode()
{
  diagramScene->setInteractionMode(InteractionMode::SIMULATION_MODE);
}

void LogiFlowWindow::addComponent()
{
  const QPoint globalPos = QCursor::pos();

  // Get cursor pos within view
  const QPoint viewPos = diagramView->mapFromGlobal(globalPos);

  // Get the position of the center of the view
  const QPoint centerViewPos = diagramView->viewport()->rect().center();

  const bool isCursorInsideView = diagramView->viewport()->rect().contains(viewPos);

  // The position at which the dialog should be displayed
  const QPoint dialogPos =
      isCursorInsideView ? globalPos : diagramView->mapToGlobal(centerViewPos);

  qDebug() << "Adding component @ " << dialogPos;

  // FIXME: If the cursor is not inside the view then the scene should be put in component
  // placing mode and the component should be placed manually.

  // TODO: When the component is selected in the dialog we should go in component placing
  // mode and repeat placing of the same component
}

void LogiFlowWindow::updateStatus()
{
  QString modeMsg = "Interaction Mode: ";

  switch (diagramScene->getInteractionMode()) {
    case DiagramScene::NORMAL_MODE:
      modeMsg += "NORMAL";
      break;
    case DiagramScene::COMPONENT_PLACING_MODE:
      modeMsg += "COMPONENT PLACING";
      break;
    case DiagramScene::WIRE_CREATION_MODE:
      modeMsg += "WIRE CREATION";
      break;
    case DiagramScene::PAN_MODE:
      modeMsg += "PAN";
      break;
    case DiagramScene::SIMULATION_MODE:
      modeMsg += "SIMULATION";
      break;
    default:
      assert(false);
  }

  statusBar()->showMessage(modeMsg);
}
