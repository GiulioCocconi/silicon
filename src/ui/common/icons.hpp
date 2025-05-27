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

#include <unordered_map>

#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QSvgRenderer>

#include <QDebug>

class Icon : public QIcon {
public:
  Icon(const QString& commonName, const QSize targetSize = QSize(32, 32));

private:
  static const inline std::unordered_map<QString, QString> commonToAwesomeMap{
      {"mouse-pointer", "arrow-pointer-solid"},
      {"pan", "arrows-up-down-left-right-solid"},
      {"chart", "chart-line-solid"},
      {"check", "check-solid"},
      {"info", "circle-info-solid"},
      {"xmark", "circle-xmark-solid"},
      {"copy", "copy-solid"},
      {"explosion", "explosion-solid"},
      {"export", "file-export-solid"},
      {"file", "file-regular"},
      {"save", "floppy-disk-solid"},
      {"open", "folder-open-regular"},
      {"link", "link-solid"},
      {"paste", "paste-solid"},
      {"plug-error", "plug-circle-xmark-solid"},
      {"play", "play-solid"},
      {"plug", "plug-solid"},
      {"plus", "plus-solid"},
      {"print", "print-solid"},
      {"undo", "rotate-left-solid"},
      {"redo", "rotate-right-solid"},
      {"rotate", "rotate-solid"},
      {"cut", "scissors-solid"},
      {"delete", "trash-solid"},
      {"xmark", "xmark-solid"}};

  static QString getPathFromCommonName(const QString& commonName);
};
