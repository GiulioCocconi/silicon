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

#include "gates.hpp"

Gate::Gate(std::vector<Wire_ptr> inputs, Wire_ptr output) {

  assert(inputs.size() != 0);

  for (auto input : inputs)
    this->inputs.push_back({input});
  this->outputs = {{output}};
}

AndGate::AndGate(std::vector<Wire_ptr> inputs, Wire_ptr output) :
  Gate(inputs, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {
    State s = State::HIGH;

    for (auto input : this->inputs)
      s = s && input[0]->getCurrentState();

    this->outputs[0][0]->setCurrentState(s, weak_from_this());
  });
}

OrGate::OrGate(std::vector<Wire_ptr> inputs, Wire_ptr output) :
  Gate(inputs, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {
    State s = State::LOW;

    for (auto input : this->inputs)
      s = s || input[0]->getCurrentState();

    this->outputs[0][0]->setCurrentState(s, weak_from_this());
  });
}

NotGate::NotGate(Wire_ptr input, Wire_ptr output) :
  Gate({input}, output)
{
  const auto a = std::make_shared<action>([this]() {
    State s = !this->inputs[0][0]->getCurrentState();
    this->outputs[0][0]->setCurrentState(s, weak_from_this());
  });

  this->inputs[0][0]->addUpdateAction(a);
}

NandGate::NandGate(std::vector<Wire_ptr> inputs, Wire_ptr output) :
  Gate(inputs, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {

    State s = State::HIGH;

    for (auto input : this->inputs)
      s = s && input[0]->getCurrentState();

    this->outputs[0][0]->setCurrentState(!s, weak_from_this());
  });

}

NorGate::NorGate(std::vector<Wire_ptr> inputs, Wire_ptr output) :
  Gate(inputs, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {

    State s = State::LOW;

    for (auto input : this->inputs)
      s = s || input[0]->getCurrentState();

    this->outputs[0][0]->setCurrentState(!s, weak_from_this());
  });
}

XorGate::XorGate(std::array<Wire_ptr, 2> inputs, Wire_ptr output) :
  Gate({inputs[0], inputs[1]}, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {
    State s = (this->inputs[0][0]->getCurrentState() ^
	       this->inputs[1][0]->getCurrentState());
    this->outputs[0][0]->setCurrentState(s, weak_from_this());
  });
}
