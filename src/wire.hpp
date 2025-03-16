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
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
#include <string>

// Each wire could hold one of three states
enum State {
  HIGH,
  LOW,
  ERROR,
};

State operator && (const State& a, const State& b);
State operator || (const State& a, const State& b);
State operator !  (const State& a);

std::string to_str(State s);

// Following SICP 3.3.4 there are no gates as devices but only wires with
// associate update actions.
using action  = std::function<void ()>;

class Wire {
protected:
  std::string          name;             // Unique name for the wire
  State                currentState;
  std::vector<Wire>    connectedWires;
  std::vector<action> updateActions;
  
public:
  Wire() {};
  Wire(std::string name);
  State       getCurrentState() const;
  void        setCurrentState(const State newState);
  std::string getName() const;
  void        addUpdateAction(const action a);
};
using Wire_ptr = std::shared_ptr<Wire>;
bool operator == (const Wire& a, const Wire& b);
