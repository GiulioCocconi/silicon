#include <string>

#include <component.hpp>

#pragma once

class Input : public Component {
public:
  Input(unsigned short outputSize = 1);
  void set(std::string str);
  void reset();
};

class Output : public Component {
public:
  Output(unsigned short inputSize = 1) : Component({inputSize}, {}, true) {};
  std::string toString();
};
