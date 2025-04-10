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
set(src_dir                        "${CMAKE_SOURCE_DIR}/src")
set(resources_dir                  "${CMAKE_SOURCE_DIR}/resources/app")

include_directories(${src_dir})

set(COMMON_SOURCE_FILES 
  ${src_dir}/core/wire.cpp
  ${src_dir}/core/gates.cpp
  ${src_dir}/core/component.cpp)


set(UI_SOURCE_FILES
  ${src_dir}/ui/common/diagramView.cpp
  ${src_dir}/ui/common/diagramScene.cpp
  ${src_dir}/ui/logiFlow/components/graphicalComponent.cpp
  ${src_dir}/ui/logiFlow/components/graphicalGates.cpp
  ${src_dir}/ui/logiFlow/logiFlowWindow.cpp)

set(CMAKE_CXX_FLAGS_DEBUG       "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

option(SILICON_BUILD_TESTS "Build silicon tests" ON)

add_compile_definitions(
    SILICON_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    SILICON_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    SILICON_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    SILICON_VERSION="${PROJECT_VERSION}"
    SILICON_DEBUG=$<CONFIG:Debug>
)

find_package(Qt6 REQUIRED COMPONENTS Widgets SvgWidgets)
qt_standard_project_setup()

qt_add_executable(Silicon
  ${src_dir}/main.cpp)

target_sources(Silicon
  PRIVATE
  ${COMMON_SOURCE_FILES}
  ${UI_SOURCE_FILES})

target_link_libraries(Silicon PRIVATE Qt6::Widgets Qt6::SvgWidgets)

qt_add_resources(Silicon "images" BASE ${resources_dir}
  FILES
  ${resources_dir}/splash.jpg)

qt_add_resources(Silicon "ANSI_icons" BASE ${resources_dir}
  FILES
  ${resources_dir}/gates/AND_ANSI.svg
  ${resources_dir}/gates/OR_ANSI.svg
  ${resources_dir}/gates/NOT_ANSI.svg
  ${resources_dir}/gates/NAND_ANSI.svg
  ${resources_dir}/gates/NOR_ANSI.svg
  ${resources_dir}/gates/XOR_ANSI.svg
)

add_compile_options(-Wall -Wextra -fsanitize=address)
add_link_options(-fsanitize=address -static-libasan)


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



