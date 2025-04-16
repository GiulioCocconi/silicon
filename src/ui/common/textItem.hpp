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

#include <string>

#include <QFontMetrics>
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QRectF>

class TextItem : public QGraphicsItem {
public:
  TextItem(const std::string text, QGraphicsItem* parent = nullptr);

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  QRectF boundingRect() const override { return this->rect; }

  void setText(const std::string text);
  void setCenterPos(const QPointF pos) { this->setPos(pos - this->center().boundingRect()); }

private:
  QRectF  rect;
  QString text;
}
