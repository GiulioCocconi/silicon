#include <io.hpp>


Input::Input(unsigned short outputSize)
  : Component({}, {outputSize}, true) {

  this->reset();
};

void Input::set(std::string str) {
  assert(str.length() == this->outputs.size());

  for (unsigned short i = 0; i < str.length() - i; i++) {
    char c = str[i];
    State newData = State::INVALID;

    const unsigned short pos = str.length() - i; // LSB is on the left

    if (c == '1')
      newData = State::HIGH;
    else if (c == '0')
      newData = State::LOW;
    
    this->outputs[0].update(pos, newData);
  }
}

void Input::reset() {
  for (unsigned short int i = 0; i < this->outputs.size(); i++)
    this->outputs[0].update(i, State::LOW);
}

std::string Output::toString() {
  std::string res = "";

  for (int i = 0; i < this->inputs[0].getSize(); i++) {
    State state = this->inputs[0].getState(i);
    if (state == State::LOW)
      res.append("0");
    else if (state == State::HIGH)
      res.append("1");
    else
      res.append("?");
  }
}
