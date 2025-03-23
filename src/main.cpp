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

#include <iostream>
#include <memory>

#include <QApplication>
#include <QMainWindow>
#include <QSplashScreen>
#include <QLabel>
#include <QIcon>
#include <QCommandLineParser>

#include <core/gates.hpp>
#include <core/wire.hpp>
#include <ui/logiFlowWindow.hpp>

#if SILICON_DEBUG
int main(int argc, char** argv) {
  auto a = std::make_shared<Wire>();
  auto b = std::make_shared<Wire>();
  auto o = std::make_shared<Wire>();

  auto g = std::make_shared<AndGate>(std::vector<Wire_ptr>{a, b}, o);

  a->forceSetCurrentState(State::ERROR);
  b->forceSetCurrentState(State::ERROR);
}

#else

int main(int argc, char** argv) {

  QApplication app(argc, argv);
  app.setApplicationName("SILICON");
  app.setStyle("Fusion");
  app.setApplicationVersion(SILICON_VERSION);

  // TODO: Create an icon!!!
  // app.setWindowIcon(QIcon)

  // Command Line Parser

  QCommandLineParser parser;
  parser.setApplicationDescription("SILICON: Simulation of Interconnected Logical Inputs, Circuits, and Output Nodes");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.process(app);



  // Splash screen
  QSplashScreen splashScreen(QPixmap(":/splash.jpg"));
  splashScreen.show();
  splashScreen.showMessage("Loading...",
			   Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

  // Force processing of events to show the splash screen immediately
  app.processEvents();


  LogiFlowWindow lfWin {};
  lfWin.show();

  splashScreen.finish(&lfWin);
  return app.exec();
}

#endif
