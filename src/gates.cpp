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

#include <gates.hpp>
#include <iostream>

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
      
    this->outputs[0][0]->setCurrentState(s);
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
      
    this->outputs[0][0]->setCurrentState(s);
  });
} 

NotGate::NotGate(Wire_ptr input, Wire_ptr output) :
  Gate({input}, output)
{
  const auto a = std::make_shared<action>([&]() {
    this->outputs[0][0]->setCurrentState(!this->inputs[0][0]->getCurrentState());
  });
  
  this->inputs[0][0]->addUpdateAction(a);
} 

NandGate::NandGate(std::vector<Wire_ptr> inputs, Wire_ptr output) :
  Gate(inputs, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {
    auto a = std::make_shared<Wire>();
    auto n = std::make_shared<Wire>();
    AndGate ag({this->inputs[0][0], this->inputs[1][0]}, a);
    NotGate ng(a, n);
    this->outputs[0][0]->setCurrentState(n->getCurrentState());
  });

}

NorGate::NorGate(std::vector<Wire_ptr> inputs, Wire_ptr output) :
  Gate(inputs, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {
    auto o = std::make_shared<Wire>();
    auto n = std::make_shared<Wire>();
    OrGate  og(this->inputs[0], o);
    NotGate ng(o, n);
    this->outputs[0][0]->setCurrentState(n->getCurrentState());
  });
}

XorGate::XorGate(std::array<Wire_ptr, 2> inputs, Wire_ptr output) :
  Gate({inputs[0], inputs[1]}, output)
{
  assert(inputs.size() >= 2);
  this->setAction([this]() {
    auto ow = std::make_shared<Wire>();
    auto nandw = std::make_shared<Wire>();
    auto outw = std::make_shared<Wire>();
    OrGate og({this->inputs[1][0], this->inputs[0][0]}, ow);
    NandGate nandg({this->inputs[0][0], this->inputs[1][0]}, nandw);
    AndGate andg({ow, nandw}, outw);
    this->outputs[0][0]->setCurrentState(outw->getCurrentState());
  });
}
