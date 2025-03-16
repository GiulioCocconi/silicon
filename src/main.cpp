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

#include <iostream>
#include <memory>

#include <gates.hpp>
#include <wire.hpp>

int main(int argc, char** argv) {
  auto a = std::make_shared<Wire>("a");
  auto b = std::make_shared<Wire>("b");
  
  auto outor = std::make_shared<Wire>("outor");
  OrGate o {{a, b}, outor};

  auto outand1 = std::make_shared<Wire>("outand");
  AndGate a1({a, b}, outand1);

  auto nand = std::make_shared<Wire>("nand");
  NotGate n1(outand1, nand);

  auto out = std::make_shared<Wire>("output");
  AndGate a2({nand, outor}, out);
  
  a->setCurrentState(State::LOW);
  b->setCurrentState(State::LOW);
  std::cout << to_str(out->getCurrentState()) << "\n";
}
