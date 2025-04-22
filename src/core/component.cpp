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

#include "component.hpp"

Component::Component(std::vector<Bus> inputs, std::vector<Bus> outputs, std::string name)
{
  this->inputs  = inputs;
  this->outputs = outputs;
  this->name    = name;
}

void Component::setInputs(std::vector<Bus> newInputs)
{
  // If I change the inputs after the component creation I need to remove the
  // update action from the former inputs.

  // If the former inputs are the same as the new inputs then do nothing:
  if (this->inputs == newInputs)
    return;

  // If the action is already defined then we should remove it from the inputs:
  if (this->act)
    for (auto bus : this->inputs)
      for (auto w : bus)
        if (w)
          w->deleteUpdateAction(this->act);

  // Then we set the new inputs and add the update action to them:
  this->inputs = newInputs;

  if (this->act)
    for (auto bus : this->inputs)
      for (auto w : bus)
        if (w)
          w->addUpdateAction(this->act);
}

void Component::setAction(action a)
{
  this->act = std::make_shared<action>(a);
  assert(this->act);

  // The action is to be set for all the inputs of the component connected to wires:
  for (auto bus : this->inputs)
    for (auto w : bus)
      if (w)
        w->addUpdateAction(this->act);
}

Component::~Component()
{
  // Remove the associated update action from all the inputs:
  for (auto bus : this->inputs)
    for (auto w : bus)
      if (w)
	w->deleteUpdateAction(this->act);
}
