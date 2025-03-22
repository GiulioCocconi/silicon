#include "logiFlowWindow.hpp"

LogiFlowWindow::LogiFlowWindow() {
  
  QWidget *centralWidget = new QWidget;
  setCentralWidget(centralWidget);

  QWidget *topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
			    "invoke a context menu</i>"));
  infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  infoLabel->setAlignment(Qt::AlignCenter);

  QWidget *bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(topFiller);
  layout->addWidget(infoLabel);
  layout->addWidget(bottomFiller);
  centralWidget->setLayout(layout);

  createActions();
  createMenus();

  QString message = tr("A context menu is available by right-clicking");
  statusBar()->showMessage(message);

  setWindowTitle(tr("Silicon LogiFlow"));
  setMinimumSize(160, 160);
}


void LogiFlowWindow::createActions() {

  newAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
		       tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, &QAction::triggered, this, &LogiFlowWindow::newFile);


  openAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
			tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing logiFlow file"));
  connect(openAct, &QAction::triggered, this, &LogiFlowWindow::open);


  saveAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
			tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the circuit to disk"));
  connect(saveAct, &QAction::triggered, this, &LogiFlowWindow::save);

  exportImageAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ImageLoading),
			 tr("&Export..."), this);
  exportImageAct->setShortcuts(QKeySequence::Print);
  exportImageAct->setStatusTip(tr("Export the circuit as an image"));
  connect(exportImageAct, &QAction::triggered, this, &LogiFlowWindow::exportImage);

  exitAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit),
			tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, &QAction::triggered, this, &QWidget::close);

  undoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo),
			tr("&Undo"), this);
  undoAct->setShortcuts(QKeySequence::Undo);
  undoAct->setStatusTip(tr("Undo the last operation"));
  connect(undoAct, &QAction::triggered, this, &LogiFlowWindow::undo);

  redoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo),
			tr("&Redo"), this);
  redoAct->setShortcuts(QKeySequence::Redo);
  redoAct->setStatusTip(tr("Redo the last operation"));
  connect(redoAct, &QAction::triggered, this, &LogiFlowWindow::redo);

  cutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCut),
		       tr("Cu&t"), this);
  cutAct->setShortcuts(QKeySequence::Cut);
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
			  "clipboard"));
  connect(cutAct, &QAction::triggered, this, &LogiFlowWindow::cut);

  copyAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy),
			tr("&Copy"), this);
  copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
			   "clipboard"));
  connect(copyAct, &QAction::triggered, this, &LogiFlowWindow::copy);

  pasteAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste),
			 tr("&Paste"), this);
  pasteAct->setShortcuts(QKeySequence::Paste);
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
			    "selection"));
  connect(pasteAct, &QAction::triggered, this, &LogiFlowWindow::paste);

  aboutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
			 tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, &QAction::triggered, this, &LogiFlowWindow::about);

}

void LogiFlowWindow::createMenus() {

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

#ifndef QT_NO_CONTEXTMENU
void LogiFlowWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU
