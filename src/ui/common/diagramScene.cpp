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

#include "diagramScene.hpp"

QPointF DiagramScene::snapToGrid(QPointF point)
{
  auto x = round(point.x() / DiagramScene::GRID_SIZE) * DiagramScene::GRID_SIZE;
  auto y = round(point.y() / DiagramScene::GRID_SIZE) * DiagramScene::GRID_SIZE;

  return QPointF(x, y);
}

void DiagramScene::drawBackground(QPainter* painter, const QRectF& rect)
{
  QPen pen;
  painter->setPen(pen);

  qreal            left = int(rect.left()) - (int(rect.left()) % DiagramScene::GRID_SIZE);
  qreal            top  = int(rect.top()) - (int(rect.top()) % DiagramScene::GRID_SIZE);
  QVector<QPointF> points;
  for (qreal x = left; x < rect.right(); x += DiagramScene::GRID_SIZE) {
    for (qreal y = top; y < rect.bottom(); y += DiagramScene::GRID_SIZE) {
      points.append(QPointF(x, y));
    }
  }
  painter->drawPoints(points.data(), points.size());
}

void DiagramScene::setInteractionMode(InteractionMode mode)
{
  emit DiagramScene::modeChanged(mode);
}
