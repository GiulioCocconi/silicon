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
#include <array>
#include <cassert>
#include <memory>
#include <string>

#include <core/wire.hpp>

class Component : public std::enable_shared_from_this<Component> {
protected:
  std::vector<Bus> inputs;
  std::vector<Bus> outputs;

  std::string name;

  action_ptr act;

public:
  Component() {};
  Component(std::vector<Bus> inputs, std::vector<Bus> outputs, std::string name);

  void setAction(action a);
  void setInputs(std::vector<Bus> newInputs);

  std::vector<Bus> getInputs() { return inputs; }
  std::vector<Bus> getOutputs() { return outputs; }
  std::string      getName() { return name; }

  virtual ~Component();
};
