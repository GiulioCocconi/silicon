#include <iostream>
#include <fmt/core.h>

#include <gates.hpp>
#include <wire.hpp>

int main(int argc, char** argv) {
  Wire a("a");
  Wire b("b");
  
  Wire outor {"outor"};
  OrGate o {{&a, &b}, &outor};

  Wire outand1 {"outand"};
  AndGate a1({&a, &b}, &outand1);

  Wire nand {"nand"};
  NotGate n1(&outand1, &nand);

  Wire out {"output"};
  AndGate a2({&nand, &outor}, &out);
  
  a.setCurrentState(State::LOW);
  b.setCurrentState(State::LOW);
  std::cout << to_str(out.getCurentState()) << "\n";
}
