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

#include "wire.hpp"

State operator && (const State& a, const State& b) {
  if (a == State::ERROR || b == State::ERROR)
    return State::ERROR;

  if (a == State::HIGH && b == State::HIGH)
    return State::HIGH;

  return State::LOW;
}

State operator || (const State& a, const State& b) {
  if (a == State::ERROR || b == State::ERROR)
    return State::ERROR;

  if (a == State::HIGH || b == State::HIGH)
    return State::HIGH;

  return State::LOW;
}

State operator ! (const State& a) {
  if (a == State::ERROR)
    return State::ERROR;

  if (a == State::LOW)
    return State::HIGH;

  return State::LOW;
}

State operator ^ (const State& a, const State& b) {
  if (a == State::ERROR || b == State::ERROR)
    return State::ERROR;

  if (a != b) return State::HIGH;

  return State::LOW;
}

std::string to_str(State s) {

  switch (s) {
  case State::HIGH:
    return "HIGH";
  case State::LOW:
    return "LOW";
  case State::ERROR:
    return "ERROR";
  }
  assert(false);
}

Wire::Wire() {
  this->currentState        = State::ERROR;
  this->updateActions       = {};
}

Wire::Wire(State s) {
  this->currentState        = s;
}

State Wire::getCurrentState() const {
  return this->currentState;
}

void Wire::forceSetCurrentState(const State newState) {
  if (this->currentState == newState) return;

  this->currentState = newState;

  for (action_ptr a : this->updateActions)
    if (a) (*a)();
}

void Wire::setCurrentState(const State newState,
			   const Component_ptr requestedBy) {

  if (!this->authorizedComponent.lock())
    this->authorizedComponent = requestedBy;

  State s = (this->authorizedComponent.lock() == requestedBy.lock())
    ? newState : State::ERROR;

  this->forceSetCurrentState(s);
}

void Wire::deleteUpdateAction(const action_ptr a) {
  const auto b   = this->updateActions.begin();
  const auto e   = this->updateActions.end();
  const auto pos = std::find(b, e, a);

  if (pos != e)
    this->updateActions.erase(pos);
}

void Wire::addUpdateAction(const action_ptr a) {
  assert(a);

  this->updateActions.push_back(a);

  // When I add the action I need to run it right away in order to make it work
  // when the state of the inputs is changed before the component is created!
  (*a)();
}

Bus::Bus(unsigned short size) {
  this->busData = std::vector<Wire_ptr>(size);

  for (auto& w : this->busData)
    w = std::make_shared<Wire>(State::LOW);
}

Bus::Bus(std::vector<Wire_ptr> busData) {
  this->busData = busData;

  for (Wire_ptr w : busData)
    if (!w) w = std::make_shared<Wire>(State::LOW);
}

Bus::Bus(std::initializer_list<Wire_ptr> initList) : busData(initList.size()) {
  size_t i = 0;
  for (auto val : initList) {
    busData[i++] = val;
  }
}


Bus::Bus(std::initializer_list<Wire> initList) : busData(initList.size()) {
  size_t i = 0;
  for (auto val : initList) {
    busData[i++] = std::make_shared<Wire>(val);
  }
}

int Bus::forceSetCurrentValue(const unsigned int value) {
  for (unsigned short i = 0; i < this->size(); i++) {
    State s = (value >> i) & 1 ? State::HIGH : State::LOW;
    this->busData[i]->forceSetCurrentState(s);
  }
  return (value >= (1u << this->size()));
}

int Bus::setCurrentValue(const unsigned int value,
			 const Component_ptr requestedBy) {

  for (unsigned short i = 0; i < this->size(); i++) {
    State s = (value >> i) & 1 ? State::HIGH : State::LOW;
    this->busData[i]->setCurrentState(s, requestedBy);
  }
  return (value >= (1u << this->size()));

}

int Bus::getCurrentValue() const {
  unsigned int res  = 0;

  for (unsigned int i = 0; i < this->size(); i++) {
    State s = this->busData[i]->getCurrentState();
    assert(s != State::ERROR);

    if (s == State::HIGH)
      res |= (1 << i);
  }
  return res;
}
