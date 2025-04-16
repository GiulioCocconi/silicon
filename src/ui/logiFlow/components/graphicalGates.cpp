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

#include "graphicalGates.hpp"

GraphicalGate::GraphicalGate(const std::weak_ptr<Gate> gate, QGraphicsItem* shape,
                             QGraphicsItem* parent)
  : GraphicalLogicComponent(gate, shape, parent)
{
  // TODO: ADD SUPPORT FOR OVER 2 INPUTS GATES
  assert(gate.lock()->getInputs().size() == 2);

  isEditable = false;

  std::vector<std::pair<std::string, QPoint>> inputVec;
  inputVec.reserve(2);

  inputVec.emplace_back("a", QPoint(-20, 10));

  inputVec.emplace_back("b", QPoint(-20, 30));

  const auto outputPoint = QPoint(80, 20);

  setPorts(inputVec, {std::pair<std::string, QPoint>{"o", outputPoint}});
}

GraphicalNot::GraphicalNot(const std::weak_ptr<NotGate> gate, QGraphicsItem* parent)
  : GraphicalLogicComponent(gate, new QGraphicsSvgItem(":/gates/NOT_ANSI.svg"), parent)
{
  isEditable = false;

  assert(gate.lock()->getInputs().size() == 1);

  isEditable = false;

  setPorts({std::pair<std::string, QPoint>{"i", QPoint(-20, 20)}},
           {std::pair<std::string, QPoint>{"o", QPoint(80, 20)}});
}
