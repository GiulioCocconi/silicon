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
#include <wire.hpp>

TEST(LogicTest, And) {
  auto a = std::make_shared<Wire>("a");
  auto b = std::make_shared<Wire>("b");
  auto o = std::make_shared<Wire>("o");
  
  AndGate g({a, b}, o);

  a->setCurrentState(State::ERROR);
  b->setCurrentState(State::ERROR);

  State outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR) << "AND(ERROR, ERROR) = " << to_str(outputState);

  b->setCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR) << "AND(ERROR, HIGH) = " << to_str(outputState);
  
  b->setCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::ERROR) << "AND(ERROR, LOW) = " << to_str(outputState);

  a->setCurrentState(State::LOW);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW) << "AND(LOW, LOW) = " << to_str(outputState);

  
  a->setCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::LOW) << "AND(HIGH, LOW) = " << to_str(outputState);

  
  b->setCurrentState(State::HIGH);
  outputState = o->getCurrentState();
  EXPECT_EQ(outputState, State::HIGH) << "AND(HIGH, HIGH) = " << to_str(outputState);
}
