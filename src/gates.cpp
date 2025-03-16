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

AndGate::AndGate(std::vector<Wire_ptr> inputs, Wire_ptr output) {
 this->inputs = inputs;
 this->output = output;

  for (Wire_ptr w : this->inputs)
    w->addUpdateAction([&]() {
      State s = State::HIGH;

      for (Wire_ptr input : this->inputs)
	s = s && input->getCurrentState();
      
      this->output->setCurrentState(s);
    });
} 


OrGate::OrGate(std::vector<Wire_ptr> inputs, Wire_ptr output) {
  this->inputs = inputs;
  this->output = output;

  for (Wire_ptr w : this->inputs)
    w->addUpdateAction([&]() {
      State s = State::LOW;

      for (Wire_ptr input : this->inputs)
	s = s || input->getCurrentState();
      
      this->output->setCurrentState(s);
    });
} 

NotGate::NotGate(Wire_ptr input, Wire_ptr output) {
  this->inputs = { input };
  this->output = output;

    this->inputs[0]->addUpdateAction([&]() {
      this->output->setCurrentState(!this->inputs[0]->getCurrentState());
    });
} 

NandGate::NandGate(std::vector<Wire_ptr> inputs, Wire_ptr output) {
  this->inputs = inputs;
  this->output = output;

  for (Wire_ptr w : this->inputs)
    w->addUpdateAction([&]() {
      auto a = std::make_shared<Wire>("a");
      auto n = std::make_shared<Wire>("n");
      AndGate ag(this->inputs, a);
      NotGate ng(a, n);
      this->output->setCurrentState(n->getCurrentState());
    });
}

NorGate::NorGate(std::vector<Wire_ptr> inputs, Wire_ptr output) {
  this->inputs = inputs;
  this->output = output;

  for (Wire_ptr w : this->inputs)
    w->addUpdateAction([&]() {
      auto o = std::make_shared<Wire>("o");
      auto n = std::make_shared<Wire>("n");
      OrGate  og(this->inputs, o);
      NotGate ng(o, n);
      this->output->setCurrentState(n->getCurrentState());
    });
}

XorGate::XorGate(std::array<Wire_ptr, 2> inputs, Wire_ptr output) {

  this->inputs = inputs;
  this->output = output;

  for (Wire_ptr w : this->inputs)
    w->addUpdateAction([&]() {
      auto ow = std::make_shared<Wire>("ow");
      auto nandw = std::make_shared<Wire>("nandw");
      auto outw = std::make_shared<Wire>("outw");
      OrGate og({this->inputs[0], this->inputs[1]}, ow);
      NandGate nandg({this->inputs[0], this->inputs[1]}, nandw);
      AndGate andg({ow, nandw}, outw);
      this->output->setCurrentState(outw->getCurrentState());
    });
}
