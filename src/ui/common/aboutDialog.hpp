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

#pragma once

#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStringBuilder>
#include <QVBoxLayout>

#include <ui/common/icons.hpp>

class AboutDialog : public QDialog {
  Q_OBJECT

public:
  explicit AboutDialog(const QString& title = "Silicon", QWidget* parent = nullptr);

private:
  void loadLicenses();

  QPlainTextEdit* licenseTextEdit;
  QPushButton*    closeButton;
  QString         licenseText = "";
};