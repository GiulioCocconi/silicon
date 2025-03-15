#pragma once
#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>
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
  State       getCurentState() const;
  void        setCurrentState(const State newState);
  std::string getName() const;
  void        addUpdateAction(const action a);
};

bool operator == (const Wire& a, const Wire& b);
