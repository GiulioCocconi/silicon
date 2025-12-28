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

#include <ranges>

#include <QAbstractItemView>
#include <QCompleter>
#include <QGraphicsProxyWidget>
#include <QKeyEvent>
#include <QLineEdit>

#include <ui/common/enums.hpp>

class ComponentSearchBox : public QGraphicsProxyWidget {
  Q_OBJECT
public:
  using SearchMap = std::map<std::string, SiliconTypes, std::less<>>;
  explicit ComponentSearchBox(
      SearchMap completionMap = {{"AND GATE", SiliconTypes::AND_GATE},
                                 {"OR GATE", SiliconTypes::OR_GATE}},
      QString title = "Insert component...", QGraphicsItem* parent = nullptr);
  void showCompleter();

  void keyPressEvent(QKeyEvent* event) override;

  void focus() { le->setFocus(Qt::OtherFocusReason); }
  void clear() { le->clear(); }

  void setCompletionMap(const SearchMap map) { this->completionMap = map; }

signals:

  void requestHide();
  void selectedComponent(SiliconTypes type, QPointF pos);

private:
  QLineEdit*         le;
  QCompleter*        completer;
  QGraphicsTextItem* titleItem;

  SearchMap completionMap;
};
