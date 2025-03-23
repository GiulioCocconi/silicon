#include "logiFlowWindow.hpp"

LogiFlowWindow::LogiFlowWindow() {

  QWidget *centralWidget = new QWidget;
  setCentralWidget(centralWidget);

  QWidget *topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QWidget *bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(topFiller);
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

  newAct         = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),     tr("&New"),       this);
  openAct        = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),    tr("&Open..."),   this);
  saveAct        = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),    tr("&Save"),      this);
  exportImageAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ImageLoading),    tr("&Export..."), this);
  exitAct        = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit), tr("E&xit"),      this);
  undoAct        = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo),        tr("&Undo"),      this);
  redoAct        = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo),        tr("&Redo"),      this);
  cutAct         = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCut),         tr("Cu&t"),       this);
  copyAct        = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy),        tr("&Copy"),      this);
  pasteAct       = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste),       tr("&Paste"),     this);
  aboutAct       = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),       tr("&About"),     this);

  newAct->setShortcuts(QKeySequence::New);
  openAct->setShortcuts(QKeySequence::Open);
  saveAct->setShortcuts(QKeySequence::Save);
  exportImageAct->setShortcuts(QKeySequence::Print);
  exitAct->setShortcuts(QKeySequence::Quit);
  undoAct->setShortcuts(QKeySequence::Undo);
  redoAct->setShortcuts(QKeySequence::Redo);
  cutAct->setShortcuts(QKeySequence::Cut);
  copyAct->setShortcuts(QKeySequence::Copy);
  pasteAct->setShortcuts(QKeySequence::Paste);


  newAct->setStatusTip(tr("Create a new file"));
  openAct->setStatusTip(tr("Open an existing logiFlow file"));
  saveAct->setStatusTip(tr("Save the circuit to disk"));
  exportImageAct->setStatusTip(tr("Export the circuit as an image"));
  exitAct->setStatusTip(tr("Exit the application"));
  undoAct->setStatusTip(tr("Undo the last operation"));
  redoAct->setStatusTip(tr("Redo the last operation"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
  aboutAct->setStatusTip(tr("Show the application's About box"));

  connect(newAct,         &QAction::triggered, this, &LogiFlowWindow::newFile);
  connect(openAct,        &QAction::triggered, this, &LogiFlowWindow::open);
  connect(saveAct,        &QAction::triggered, this, &LogiFlowWindow::save);
  connect(exportImageAct, &QAction::triggered, this, &LogiFlowWindow::exportImage);
  connect(exitAct,        &QAction::triggered, this, &QWidget::close);
  connect(undoAct,        &QAction::triggered, this, &LogiFlowWindow::undo);
  connect(redoAct,        &QAction::triggered, this, &LogiFlowWindow::redo);
  connect(cutAct,         &QAction::triggered, this, &LogiFlowWindow::cut);
  connect(copyAct,        &QAction::triggered, this, &LogiFlowWindow::copy);
  connect(pasteAct,       &QAction::triggered, this, &LogiFlowWindow::paste);
  connect(aboutAct,       &QAction::triggered, this, &LogiFlowWindow::about);

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
