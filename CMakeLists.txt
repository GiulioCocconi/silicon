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

cmake_minimum_required(VERSION 3.26)

project("SILICON" 
	VERSION 0.1.0 # As defined at semver.org
	LANGUAGES CXX)

include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(FetchContent)
include(GoogleTest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(src_dir "${CMAKE_SOURCE_DIR}/src")

include_directories(${src_dir})

set(COMMON_SOURCE_FILES 
  ${src_dir}/wire.cpp
  ${src_dir}/gates.cpp)

option(SILICON_BUILD_TESTS "Build silicon tests" ON)

add_executable(logiFlow 
  ${src_dir}/main.cpp)

target_sources(logiFlow
  PRIVATE
  ${COMMON_SOURCE_FILES})

target_compile_options(logiFlow PRIVATE -Wall -Wextra)
target_compile_features(logiFlow PRIVATE cxx_std_20)


if(SILICON_BUILD_TESTS)
  find_package(GTest QUIET)

  if(NOT GTest_FOUND)
    message(STATUS "googletest not found, fetching with FetchContent...")

    FetchContent_Declare(
      GTest
      GIT_REPOSITORY https://github.com/google/googletest.git
    )
    FetchContent_MakeAvailable(GTest)

  else()
    message(STATUS "GOOGLETEST ALREADY INSTALLED!")
  endif()

  add_subdirectory("${CMAKE_SOURCE_DIR}/tests")
  enable_testing()
endif()



