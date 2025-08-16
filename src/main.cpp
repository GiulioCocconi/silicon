/*
 Copyright (c) 2025. Giulio Cocconi

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

#include <QApplication>
#include <QCommandLineParser>
#include <QFont>
#include <QFontDatabase>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QSplashScreen>

#include <core/gates.hpp>
#include <core/wire.hpp>
#include <ui/common/icons.hpp>
#include <ui/logiFlow/logiFlowWindow.hpp>

int siliconMain(int argc, char** argv)
{
  const QApplication app(argc, argv);
  app.setApplicationName("SILICON");
  app.setStyle("Fusion");
  app.setApplicationVersion(SILICON_VERSION);

  // LOAD THE FONTS
  QFontDatabase::addApplicationFont(":/fonts/Chango.ttf");
  QFontDatabase::addApplicationFont(":/fonts/Quicksand.ttf");
  QFontDatabase::addApplicationFont(":/fonts/NovaMono.ttf");

  app.setFont(QFont("Quicksand", app.font().pointSize() * 1.2, QFont::Medium));

  QApplication::setWindowIcon(Icon("silicon", {QSize(32, 32), QSize(128, 128)}));

  // Command Line Parser

  QCommandLineParser parser;
  parser.setApplicationDescription("SILICON: Simulation of Interconnected Logical\
  Inputs, Circuits, and Output Nodes");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.process(app);

  // Splash screen
  QSplashScreen splashScreen(QPixmap(":/splash.jpg"));
  splashScreen.show();
  splashScreen.showMessage("Loading...", Qt::AlignBottom | Qt::AlignHCenter, Qt::white);

  // Force processing of events to show the splash screen immediately
  app.processEvents();

  LogiFlowWindow lfWin{};
  lfWin.show();

  splashScreen.finish(&lfWin);
  return app.exec();
}

#if 0
#  include <iostream>
#  include <memory>
#  include <ui/components/graphicalGates.cpp>

#  include <QGraphicsEllipseItem>
int main(int argc, char** argv) {
  auto a = std::make_shared<Wire>(State::HIGH);
  auto o = std::make_shared<Wire>();
  auto ag = std::make_shared<AndGate>(std::vector<Wire_ptr>{a,a}, o);
  GraphicalAnd* graphicalAnd = new GraphicalAnd(ag, nullptr);

  std::cout << "Hey!";
}

#else
int main(int argc, char** argv)
{
  return siliconMain(argc, argv);
}
#endif
