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

cmake_minimum_required(VERSION 3.25)

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
set(resources_dir "${CMAKE_SOURCE_DIR}/resources/app")

include_directories(${src_dir})

set(COMMON_SOURCE_FILES
        ${src_dir}/core/wire.cpp
        ${src_dir}/core/gates.cpp
        ${src_dir}/core/component.cpp)

set(EXTRA_COMPONENTS_SOURCE_FILES
        ${src_dir}/extraComponents/arithmetic.cpp)

set(UI_SOURCE_FILES
        ${src_dir}/ui/common/componentSearchBox.cpp
        ${src_dir}/ui/common/diagramView.cpp
        ${src_dir}/ui/common/diagramScene.cpp
        ${src_dir}/ui/common/graphicalWire.cpp
        ${src_dir}/ui/common/graphicalComponent.cpp
        ${src_dir}/ui/common/icons.cpp
        ${src_dir}/ui/logiFlow/components/graphicalLogicComponent.cpp
        ${src_dir}/ui/logiFlow/components/graphicalIO.cpp
        ${src_dir}/ui/logiFlow/components/graphicalGates.cpp
        ${src_dir}/ui/logiFlow/logiFlowWindow.cpp)

set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SILICON_BUILD_TESTS "Build silicon tests" ON)

add_compile_definitions(
        SILICON_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        SILICON_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        SILICON_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        SILICON_VERSION="${PROJECT_VERSION}"
        SILICON_DEBUG=$<CONFIG:Debug>
)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if (CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
            ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif ()

add_compile_options(-Wall -Wextra -Wno-unused-parameter -fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined -static-libasan)

find_package(Qt6 REQUIRED COMPONENTS Widgets SvgWidgets)
qt_standard_project_setup()

qt_add_executable(Silicon
        ${src_dir}/main.cpp)

target_sources(Silicon
        PRIVATE
        ${COMMON_SOURCE_FILES}
        ${EXTRA_COMPONENTS_SOURCE_FILES}
        ${UI_SOURCE_FILES})

target_link_libraries(Silicon PRIVATE Qt6::Widgets Qt6::SvgWidgets)

qt_add_resources(Silicon "images" BASE ${resources_dir}
        FILES
        ${resources_dir}/splash.jpg)


qt_add_resources(Silicon "fonts" BASE ${resources_dir}
        FILES
        ${resources_dir}/fonts/Chango.ttf
        ${resources_dir}/fonts/Quicksand.ttf
        ${resources_dir}/fonts/NovaMono.ttf)

qt_add_resources(Silicon "icons" BASE ${resources_dir}
        FILES
        ${resources_dir}/icons/arrow-pointer-solid.svg
        ${resources_dir}/icons/arrows-up-down-left-right-solid.svg
        ${resources_dir}/icons/chart-line-solid.svg
        ${resources_dir}/icons/check-solid.svg
        ${resources_dir}/icons/circle-info-solid.svg
        ${resources_dir}/icons/circle-xmark-solid.svg
        ${resources_dir}/icons/copy-solid.svg
        ${resources_dir}/icons/explosion-solid.svg
        ${resources_dir}/icons/file-export-solid.svg
        ${resources_dir}/icons/file-regular.svg
        ${resources_dir}/icons/floppy-disk-solid.svg
        ${resources_dir}/icons/folder-open-regular.svg
        ${resources_dir}/icons/link-solid.svg
        ${resources_dir}/icons/paste-solid.svg
        ${resources_dir}/icons/play-solid.svg
        ${resources_dir}/icons/plug-circle-xmark-solid.svg
        ${resources_dir}/icons/plug-solid.svg
        ${resources_dir}/icons/plus-solid.svg
        ${resources_dir}/icons/print-solid.svg
        ${resources_dir}/icons/rotate-left-solid.svg
        ${resources_dir}/icons/rotate-right-solid.svg
        ${resources_dir}/icons/rotate-solid.svg
        ${resources_dir}/icons/scissors-solid.svg
        ${resources_dir}/icons/trash-solid.svg
        ${resources_dir}/icons/xmark-solid.svg
)

qt_add_resources(Silicon "ANSI_icons" BASE ${resources_dir}
        FILES
        ${resources_dir}/gates/AND_ANSI.svg
        ${resources_dir}/gates/OR_ANSI.svg
        ${resources_dir}/gates/NOT_ANSI.svg
        ${resources_dir}/gates/NAND_ANSI.svg
        ${resources_dir}/gates/NOR_ANSI.svg
        ${resources_dir}/gates/XOR_ANSI.svg
)

qt_add_resources(Silicon "other_components" BASE ${resources_dir}
        FILES
        ${resources_dir}/other_components/input_off.svg
        ${resources_dir}/other_components/output_off.svg
        ${resources_dir}/other_components/input_on.svg
        ${resources_dir}/other_components/output_on.svg
)


if (SILICON_BUILD_TESTS)
    find_package(GTest QUIET)

    if (NOT GTest_FOUND)
        message(STATUS "googletest not found, fetching with FetchContent...")

        FetchContent_Declare(
                GTest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG main
        )
        FetchContent_MakeAvailable(GTest)

    else ()
        message(STATUS "GOOGLETEST ALREADY INSTALLED!")
    endif ()

    add_subdirectory("${CMAKE_SOURCE_DIR}/tests")
    enable_testing()
endif ()
