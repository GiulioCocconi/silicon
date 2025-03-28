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
#include <format>
#include <initializer_list>
#include <iterator>

// Each wire could hold one of three states
enum State {
  HIGH,
  LOW,
  ERROR,
};

State operator && (const State& a, const State& b);
State operator || (const State& a, const State& b);
State operator ^ (const State& a, const State& b);
State operator !  (const State& a);

std::string to_str(State s);

// Following SICP 3.3.4 there are no gates as devices but only wires with
// associate update actions.
using action        = std::function<void ()>;
using action_ptr    = std::shared_ptr<action>;

class Component;
using Component_ptr = std::weak_ptr<Component>;

class Wire {
private:
  State                         currentState;
  std::vector<action_ptr>       updateActions;
  Component_ptr                 authorizedComponent;

public:
  Wire();
  Wire(State s);

  State getCurrentState() const;
  void  forceSetCurrentState(const State newState);

  void  setCurrentState(const State newState,
			const Component_ptr requestedBy);

  void  addUpdateAction(const action_ptr a);
  void  deleteUpdateAction(const action_ptr a);
};

using Wire_ptr = std::shared_ptr<Wire>;

class Bus {
private:
  std::vector<Wire_ptr> busData;

public:
  Bus() {};
  Bus(unsigned short size);
  Bus(std::vector<Wire_ptr> busData);
  Bus(std::initializer_list<Wire> initList);
  Bus(std::initializer_list<Wire_ptr> initList);
  int forceSetCurrentValue(const unsigned int value);

  int setCurrentValue(const unsigned int value,
		      const Component_ptr requestedBy);

  int getCurrentValue() const;

  Wire_ptr& operator[](unsigned short index)       { return this->busData.at(index);        }
  operator std::vector<Wire_ptr>()           const { return this->busData;                  }
  operator std::vector<Wire_ptr>()                 { return this->busData;                  }

  auto begin()                                     { return this->busData.begin();          }
  auto end()                                       { return this->busData.end();            }

  [[nodiscard]] auto size()                        { return this->busData.size();           }
  [[nodiscard]] auto size()                  const { return this->busData.size();           }
  
  bool operator==(const Bus& other)          const { return this->busData == other.busData; }

};

