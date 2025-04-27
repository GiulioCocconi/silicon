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

#include "graphicalIO.hpp"

// TODO: IMPLEMENT COMPONENT LOGIC

GraphicalInputSingle::GraphicalInputSingle(std::string name, QGraphicsItem* parent)
  : GraphicalLogicComponent(std::make_shared<DummyInputComponent>(Bus(1), name),
                            new QGraphicsSvgItem(":/other_components/input_off.svg"),
                            parent)
{
  isEditable = false;

  setPorts({}, {std::pair<std::string, QPoint>{"o", QPoint(20, 60)}});
  setState(LOW, true);
}

void GraphicalInputSingle::toggle()
{
  setState(!getState());
}

void GraphicalInputSingle::setState(State state, bool force)
{
  if (!force && state == getState())
    return;

  QString shapePath = (state == HIGH) ? ON_SHAPE_PATH : OFF_SHAPE_PATH;
  setShape(new QGraphicsSvgItem(shapePath));

  this->getComponent()->getOutputs()[0].setCurrentValue(state == HIGH,
                                                        getComponent()->weak_from_this());

  // Make sure we actually changed the state
  assert(state == getState());
}

State GraphicalInputSingle::getState()
{
  const int value = this->getComponent()->getOutputs()[0].getCurrentValue();
  return (value == 1) ? State::HIGH : State::LOW;
}
