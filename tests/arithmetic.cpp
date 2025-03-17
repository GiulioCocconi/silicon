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

#include <vector>

#include <arithmetic.hpp>


TEST(ArithmeticTest, HalfAdderCase) {
  auto a    = std::make_shared<Wire>(State::LOW);
  auto b    = std::make_shared<Wire>(State::LOW);
  auto sum  = std::make_shared<Wire>();
  auto cout = std::make_shared<Wire>();
  
  HalfAdder ha({a,b}, sum, cout);
  ASSERT_EQ(sum->getCurrentState(),  State::LOW);
  ASSERT_EQ(cout->getCurrentState(), State::LOW);

  b->setCurrentState(State::HIGH);
  ASSERT_EQ(sum->getCurrentState(),  State::HIGH);
  ASSERT_EQ(cout->getCurrentState(), State::LOW);

  a->setCurrentState(State::HIGH);
  ASSERT_EQ(sum->getCurrentState(),  State::LOW);
  ASSERT_EQ(cout->getCurrentState(), State::HIGH);
}

TEST(ArithmeticTest, AdderNBitsFromComponents) {

  auto a                 = Bus(4);
  auto b                 = Bus(4);
  auto sum               = Bus(4);
  
  auto partialCarryWires = Bus(5); // We need one more!
  
  std::vector<FullAdder> fullAdders;
  fullAdders.reserve(4);

  Wire_ptr cout = partialCarryWires[4];

  // The first Carry-in should be LOW (that's the default value
  // for Busses so we don't need to explicitly set it).
  assert(partialCarryWires[0]->getCurrentState() == State::LOW);
    
  for (int i = 0; i < 4; i++)
    fullAdders.emplace_back(std::array<Wire_ptr, 2>{a[i], b[i]},
			      partialCarryWires[i],
			      sum[i], partialCarryWires[i+1]);


  a.setCurrentValue(0);
  b.setCurrentValue(0);
  
  ASSERT_EQ(cout->getCurrentState(), State::LOW);
  ASSERT_EQ(sum.getCurrentValue(),   0);
  
  a.setCurrentValue(0b1100);
  b.setCurrentValue(0b0011);
  
  ASSERT_EQ(cout->getCurrentState(), State::LOW);
  ASSERT_EQ(sum.getCurrentValue(),   0b1111);

  b.setCurrentValue(0b1100);
  a.setCurrentValue(0b0011);

  ASSERT_EQ(cout->getCurrentState(), State::LOW);
  ASSERT_EQ(sum.getCurrentValue(),   0b1111);

  a.setCurrentValue(0b1111);
  b.setCurrentValue(0b0001);
  
  ASSERT_EQ(cout->getCurrentState(), State::HIGH);
  ASSERT_EQ(sum.getCurrentValue(),   0);
}

TEST(ArithmeticTest, AdderNBitsAtomic) {
  auto a                 = Bus(4);
  auto b                 = Bus(4);
  auto sum               = Bus(4);
  auto cout              = std::make_shared<Wire>();

  AdderNBits adder({a, b}, sum, cout);
  
  a.setCurrentValue(0);
  b.setCurrentValue(0);

  ASSERT_EQ(cout->getCurrentState(), State::LOW);
  ASSERT_EQ(sum.getCurrentValue(),   0);
  
  a.setCurrentValue(0b1100);
  b.setCurrentValue(0b0011);
  
  ASSERT_EQ(cout->getCurrentState(), State::LOW);
  ASSERT_EQ(sum.getCurrentValue(),   0b1111);

  b.setCurrentValue(0b1100);
  a.setCurrentValue(0b0011);

  ASSERT_EQ(cout->getCurrentState(), State::LOW);
  ASSERT_EQ(sum.getCurrentValue(),   0b1111);

  a.setCurrentValue(0b1111);
  b.setCurrentValue(0b0001);
  
  ASSERT_EQ(cout->getCurrentState(), State::HIGH);
  ASSERT_EQ(sum.getCurrentValue(),   0);

}
