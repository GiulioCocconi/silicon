#  Copyright (C) 2025 Giulio Cocconi

#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

enable_testing()
add_executable(logic_tests logic.cpp)
add_executable(arithmetic_tests arithmetic.cpp)

target_link_libraries(logic_tests GTest::gtest_main)
target_link_libraries(arithmetic_tests GTest::gtest_main)

target_sources(logic_tests
  PRIVATE
  ${COMMON_SOURCE_FILES})


target_sources(arithmetic_tests
  PRIVATE
  ${src_dir}/extraComponents/arithmetic.cpp
  ${COMMON_SOURCE_FILES})

gtest_discover_tests(logic_tests)
gtest_discover_tests(arithmetic_tests)





