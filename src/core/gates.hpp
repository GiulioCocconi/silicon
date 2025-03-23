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

#pragma once
#include <cassert>
#include <memory>
#include <array>
#include <core/wire.hpp>
#include <core/component.hpp>

class Gate : public Component {
public:
  using Component::setAction;
  Gate(std::vector<Wire_ptr> inputs, Wire_ptr output);
  Gate() {};
};


class AndGate : public Gate {
public:
  AndGate(std::vector<Wire_ptr> inputs, Wire_ptr output);
};


class OrGate : public Gate {
public:
  OrGate(std::vector<Wire_ptr> inputs, Wire_ptr output);
};

class NotGate : public Gate {
public:
  NotGate(Wire_ptr input, Wire_ptr output);
};

class NandGate : public Gate {
public:
  NandGate(std::vector<Wire_ptr> inputs, Wire_ptr output);
};


class NorGate : public Gate {
public:
  NorGate(std::vector<Wire_ptr> inputs, Wire_ptr output);
};

class XorGate : public Gate {
public:
  XorGate(std::array<Wire_ptr,2> inputs, Wire_ptr output);
};
