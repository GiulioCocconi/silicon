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
add_executable(utils_tests utils.cpp)



target_sources(logic_tests
        PRIVATE
        ${COMMON_SOURCE_FILES})


target_sources(arithmetic_tests
        PRIVATE
        ${src_dir}/extraComponents/arithmetic.cpp
        ${COMMON_SOURCE_FILES})

target_sources(utils_tests
        PRIVATE
        ${src_dir}/extraComponents/utils.cpp
        ${COMMON_SOURCE_FILES})

foreach (target logic_tests arithmetic_tests utils_tests)
    target_link_libraries(${target} GTest::gtest_main GTest::gtest)
    gtest_discover_tests(${target})
endforeach ()