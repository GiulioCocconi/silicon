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

#include "componentSearchBox.hpp"
ComponentSearchBox::ComponentSearchBox(int start, int end, QString title,
                                           QGraphicsItem* parent)
  : QGraphicsProxyWidget(parent)
{
  this->le = new QLineEdit();

  le->setFixedWidth(200);
  le->setFixedHeight(30);

  this->setWidget(le);

  this->setPos(0,0);


  titleItem = new QGraphicsTextItem(this);
  titleItem->setPlainText(title);

  // Calculate font baseline offset
  QFontMetrics fm(titleItem->font());

  titleItem->setPos(0, -fm.ascent()*2);

  titleItem->setZValue(100);

}

void ComponentSearchBox::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    emit requestHide();
    return;
  }

  const QString text = event->text();

  if (!text.isEmpty() && text.at(0).isLetter()) {
    QKeyEvent newEvent(event->type(), event->key(), event->modifiers(), text.toUpper(),
                       event->isAutoRepeat(), event->count());
    QGraphicsProxyWidget::keyPressEvent(&newEvent);
    return;
  }
  QGraphicsProxyWidget::keyPressEvent(event);
}
