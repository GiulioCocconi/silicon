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
#include <arithmetic.hpp>

int main(int argc, char** argv) {
  std::cout << "Silicon (Simulation of Interconnected Logical Inputs, Circuits, and Output Nodes)"
	    << "\n";

  auto a                 = Bus(4);
  auto b                 = Bus(4);
  auto sum               = Bus(4);
  auto cout              = std::make_shared<Wire>();
  
  AdderNBits adder({ a,b }, sum, cout);


  a.setCurrentValue(0b0001);
  b.setCurrentValue(0b1111);


  int value = sum.getCurrentValue();
  
  std::cout << "Hey!\n"; 
}
