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

// FIXME!!!!

GraphicalGate::GraphicalGate(const std::weak_ptr<Gate> gate,
			     QGraphicsItem*            shape,
			     std::vector<QPoint>       inputPortsPos,
			     QPoint                    outputPortPos,
			     QGraphicsItem*            parent)
  : GraphicalComponent(gate, shape, parent)
{
  // TODO: ADD SUPPORT FOR OVER 2 INPUTS GATES
  assert(gate.lock()->getInputs().size() == 2);
  assert(inputPortsPos.size() == 2);


  isEditable = false;
  
  std::vector<std::pair<std::string, QPoint>> inputVec;
  inputVec.reserve(2);
  
  inputVec.emplace_back("a",
			 inputPortsPos[0]);
  
  inputVec.emplace_back("b",
			 inputPortsPos[1]);

  setPorts(inputVec,
	   {std::pair<std::string, QPoint>{"o", outputPortPos}});
  
}

GraphicalAnd::GraphicalAnd(const std::weak_ptr<AndGate> gate,
			   QGraphicsItem*               parent)
  : GraphicalGate(gate,
		  new QGraphicsSvgItem(":/gates/AND_ANSI.svg"),
		  {QPoint(0, 10), QPoint(0, 30)},
		  QPoint(40, 20),
		  parent) { }
