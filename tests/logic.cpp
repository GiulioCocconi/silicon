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

#include "tests.hpp"

TEST(LogicTest, And) {
  auto a = std::make_shared<Wire>();
  auto b = std::make_shared<Wire>();
  auto o = std::make_shared<Wire>();

  auto g = std::make_shared<AndGate>(std::vector<Wire_ptr>{a, b}, o);

  a->forceSetCurrentState(State::ERROR);
  b->forceSetCurrentState(State::ERROR);

  State outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "AND(ERROR, ERROR) = " << to_str(outputState);

  b->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "AND(ERROR, HIGH) = " << to_str(outputState);

  b->forceSetCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "AND(ERROR, LOW) = " << to_str(outputState);

  a->forceSetCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW)
    << "AND(LOW, LOW) = " << to_str(outputState);


  a->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW)
    << "AND(HIGH, LOW) = " << to_str(outputState);


  b->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::HIGH)
    << "AND(HIGH, HIGH) = " << to_str(outputState);
}

TEST(LogicTest, Or) {
  auto a = std::make_shared<Wire>();
  auto b = std::make_shared<Wire>();
  auto o = std::make_shared<Wire>();

  auto g = std::make_shared<OrGate>(std::vector<Wire_ptr>{a, b}, o);

  a->forceSetCurrentState(State::ERROR);
  b->forceSetCurrentState(State::ERROR);

  State outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "OR(ERROR, ERROR) = " << to_str(outputState);

  b->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "OR(ERROR, HIGH) = " << to_str(outputState);

  b->forceSetCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "OR(ERROR, LOW) = " << to_str(outputState);

  a->forceSetCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW)
    << "OR(LOW, LOW) = " << to_str(outputState);


  a->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::HIGH)
    << "OR(HIGH, LOW) = " << to_str(outputState);


  b->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::HIGH)
    << "OR(HIGH, HIGH) = " << to_str(outputState);
}

TEST(LogicTest, Xor) {
  auto a = std::make_shared<Wire>();
  auto b = std::make_shared<Wire>();
  auto o = std::make_shared<Wire>();


  auto g = std::make_shared<XorGate>(std::array<Wire_ptr, 2>{a, b}, o);

  a->forceSetCurrentState(State::ERROR);
  b->forceSetCurrentState(State::ERROR);

  State outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "XOR(ERROR, ERROR) = " << to_str(outputState);

  b->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "XOR(ERROR, HIGH) = " << to_str(outputState);

  b->forceSetCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR)
    << "XOR(ERROR, LOW) = " << to_str(outputState);

  a->forceSetCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW)
    << "XOR(LOW, LOW) = " << to_str(outputState);


  a->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::HIGH)
    << "XOR(HIGH, LOW) = " << to_str(outputState);


  b->forceSetCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW)
    << "XOR(HIGH, HIGH) = " << to_str(outputState);
}

TEST(LogicTest, CircuitEditing1) {
  auto a = std::make_shared<Wire>();
  auto b = std::make_shared<Wire>();

  auto o = std::make_shared<Wire>(State::HIGH);

  a->forceSetCurrentState(State::HIGH);
  b->forceSetCurrentState(State::LOW);

  {

    auto g = std::make_shared<XorGate>(std::array<Wire_ptr, 2>{a, b}, o);
    EXPECT_EQ(o->getCurrentState(), State::HIGH);
  }

  {

    auto g = std::make_shared<AndGate>(std::vector<Wire_ptr>{a, b}, o);
    EXPECT_EQ(o->getCurrentState(), State::LOW);
  }

  {
    b->forceSetCurrentState(State::HIGH);
    auto g = std::make_shared<OrGate>(std::vector<Wire_ptr>{a, b}, o);
    EXPECT_EQ(o->getCurrentState(), State::HIGH);
  }
}

TEST(LogicTest, CircuitEditing2) {
  auto a = std::make_shared<Wire>(State::HIGH);
  auto b = std::make_shared<Wire>(State::HIGH);
  auto c = std::make_shared<Wire>(State::HIGH);
  
  auto o = std::make_shared<Wire>();

  auto ag = std::make_shared<AndGate>(std::vector<Wire_ptr>{a, b}, o);
  ag->setInputs({{c}, {b}});

  a->forceSetCurrentState(State::ERROR); // Should not influence the gate anymore
  
  EXPECT_EQ(o->getCurrentState(), State::HIGH);

  c->forceSetCurrentState(State::LOW);
  EXPECT_EQ(o->getCurrentState(), State::LOW);
}
