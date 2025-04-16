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
#include <QGraphicsScene>
#include <QPainter>
#include <QRect>

class DiagramScene : public QGraphicsScene {
  Q_OBJECT
public:
  enum InteractionMode {
    NORMAL_MODE,
    PAN_MODE,
    WIRE_CREATION_MODE,
    COMPONENT_PLACING_MODE,
    SIMULATION_MODE,
  };

  explicit DiagramScene(QObject* parent = nullptr) : QGraphicsScene(parent) {}

  void setInteractionMode(InteractionMode mode);

  static QPointF snapToGrid(QPointF point);

  static const int GRID_SIZE = 10;

signals:
  void modeChanged(InteractionMode mode);

private:
  void            drawBackground(QPainter* painter, const QRectF& rect) override;
  InteractionMode currentInteractionMode = InteractionMode::NORMAL_MODE;
};
using InteractionMode = DiagramScene::InteractionMode;
