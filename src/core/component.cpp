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


Component::Component(std::vector<Bus> inputs,
		     std::vector<Bus> outputs)
{
  this->inputs = inputs;
  this->outputs = outputs;


  for (auto inputBus : inputs)
    for (auto w : inputBus)
      if (!w)
	assert(false);
  
  for (auto outputBus : outputs)
    for (auto w : outputBus)
      if (!w)
	assert(false);
  
}

void Component::setAction(action a) {
  this->act = std::make_shared<action>(a);
  assert(this->act);
  
  for (auto bus : this->inputs)
    for (auto w : bus)
      w->addUpdateAction(this->act);
}

Component::~Component() {
  for (auto bus : this->inputs)
    for (auto w : bus)
      w->deleteUpdateAction(this->act);
}

