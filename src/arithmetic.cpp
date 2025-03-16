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

#include <arithmetic.h>
HalfAdder::HalfAdder(std::array<Wire_ptr, 2> inputs, Wire_ptr sum, Wire_ptr cout) {
  this->inputs = inputs;
  this->sum = sum;
  this->cout = cout;

  for (auto w : this->inputs)
    w->addUpdateAction([&]() {
      XorGate xg(this->inputs, this->sum);
      AndGate ag(this->inputs, this->cout);
    });
}

FullAdder::FullAdder(std::array<Wire_ptr, 2> inputs, Wire_ptr cin,
		     Wire_ptr sum, Wire_ptr cout) {
  this->inputs = inputs;
  this->cin = cin;
  this->sum = sum;
  this->cout = cout;
}
