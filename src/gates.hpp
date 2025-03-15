#pragma once
#include <cassert>
#include <memory>
#include <wire.hpp>

class Gate {
protected:
  std::vector<Wire*> inputs;
  Wire* output;
public:
  Gate() {};
};

class AndGate : public Gate {
public:
  AndGate(std::vector<Wire*> inputs, Wire* output);
};


class OrGate : public Gate {
public:
  OrGate(std::vector<Wire*> inputs, Wire* output);
};

class NotGate : public Gate {
public:
  NotGate(Wire* input, Wire* output);
};

class NandGate : public Gate {
public:
  NandGate(std::vector<Wire*> inputs, Wire* output);
};


class NorGate : public Gate {
public:
  NorGate(std::vector<Wire*> inputs, Wire* output);
};

class XorGate : public Gate {
public:
  XorGate(std::vector<Wire*> inputs, Wire* output);
};
