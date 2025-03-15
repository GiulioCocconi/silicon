#include <wire.hpp>
#include <iostream>

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

Wire::Wire(std::string name) {
  this->name           = name;
  this->currentState   = State::ERROR;
  this->connectedWires = {};
  this->updateActions  = {};
}


std::string Wire::getName() const { return this->name; }

bool operator == (const Wire& a, const Wire& b) {
  // Names *must* be unique identifiers within a circuit so we can check for name equality
  return (a.getName() == b.getName());
}

State Wire::getCurentState() const {
  return this->currentState;
}

void Wire::setCurrentState(const State newState) {
  if (this->currentState == newState)
    return;

  this->currentState = newState;

  for (action a : this->updateActions)
    a();
}

void Wire::addUpdateAction(const std::function<void ()> a) {
  this->updateActions.push_back(a);
}

