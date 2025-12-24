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

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(src_dir "${CMAKE_SOURCE_DIR}/src")
set(resources_dir "${CMAKE_SOURCE_DIR}/resources/app")

include_directories(${src_dir})

set(USING_NIX FALSE CACHE INTERNAL "Is the Nix package manager present?")

if (APPLE)
    message(WARNING "Building on Apple devices is untested, things might break")
elseif (UNIX)
    find_program(NIX_EXECUTABLE nix)

    if (NIX_EXECUTABLE)
        set(USING_NIX TRUE CACHE INTERNAL "Is the Nix package manager present?" FORCE)
    endif ()

    if (USING_NIX AND NOT DEFINED ENV{IN_NIX_SHELL})
        message(WARNING "Nix detected, but not running inside a nix-shell/nix develop environment.")
    endif ()
endif ()

if (MSVC)
    message(FATAL_ERROR "MSVC is not currently supported!")
endif ()

option(SILICON_USE_VCPKG "Bootstrap and use bundled vcpkg for dependencies (if using Nix, this option has no effect" ON)

if (SILICON_USE_VCPKG AND NOT USING_NIX)
    message(STATUS "SILICON_USE_VCPKG is ON. Bootstrapping bundled vcpkg...")

    set(ENV{VCPKG_USE_SYSTEM_BINARIES} "1")

    # Download Vcpkg source
    FetchContent_Declare(
            vcpkg
            GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
            GIT_TAG 2025.12.12 # Pinning ensures reproducible builds (when is changed then the latest commit hash should be put in vcpkg.json)
    )
    FetchContent_MakeAvailable(vcpkg)

    # Determine script path
    if (WIN32)
        set(VCPKG_BOOTSTRAP_SCRIPT "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.bat")
        set(VCPKG_EXECUTABLE "${vcpkg_SOURCE_DIR}/vcpkg.exe")

        if (CMAKE_GENERATOR MATCHES "MinGW" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "" FORCE)
            set(VCPKG_HOST_TRIPLET "x64-mingw-dynamic" CACHE STRING "" FORCE)
        endif ()
    else ()
        set(VCPKG_BOOTSTRAP_SCRIPT "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.sh")
        set(VCPKG_EXECUTABLE "${vcpkg_SOURCE_DIR}/vcpkg")
    endif ()

    # Run Bootstrap if executable is missing
    if (NOT EXISTS "${VCPKG_EXECUTABLE}")
        message(STATUS "Running vcpkg bootstrap script...")
        execute_process(
                COMMAND "${VCPKG_BOOTSTRAP_SCRIPT}" -disableMetrics
                WORKING_DIRECTORY "${vcpkg_SOURCE_DIR}"
                RESULT_VARIABLE VCPKG_BOOTSTRAP_RESULT
        )
        if (NOT VCPKG_BOOTSTRAP_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to bootstrap vcpkg.")
        endif ()
    endif ()

    # Enable Manifest Mode (uses vcpkg.json)
    set(VCPKG_MANIFEST_INSTALL ON)
    set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}")

    # Inject toolchain so find_package works
    include("${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")
    set(X_VCPKG_APPLOCAL_COPY_DLLS ON CACHE INTERNAL "")

endif ()

function(enable_runtime_dll_copy_qt target)
    if (WIN32)
        # 1. Locate windeployqt.exe
        # We find it relative to the Qt6::qmake location provided by find_package(Qt6)
        if (NOT TARGET Qt6::qmake)
            find_package(Qt6 REQUIRED COMPONENTS Core)
        endif ()

        get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

        if (WINDEPLOYQT_EXECUTABLE)
            message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXECUTABLE}")

            # 2. Add post-build step to run windeployqt
            # --compiler-runtime: Automatically copies libstdc++, libgcc_s, etc. (replaces your MinGW manual logic)
            # --no-translations: Optional, skips copying .qm files to keep the folder clean
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --verbose 1
                    --compiler-runtime
                    --dir "$<TARGET_FILE_DIR:${target}>"
                    "$<TARGET_FILE:${target}>"
                    COMMENT "Deploying Qt dependencies for ${target}..."
                    COMMAND_EXPAND_LISTS
            )

        else ()
            message(WARNING "windeployqt not found")
        endif ()
    endif ()
endfunction()

set(COMMON_SOURCE_FILES
        ${src_dir}/core/wire.cpp
        ${src_dir}/core/gates.cpp
        ${src_dir}/core/component.cpp)

set(EXTRA_COMPONENTS_SOURCE_FILES
        ${src_dir}/extraComponents/arithmetic.cpp
        ${src_dir}/extraComponents/utils.cpp)

set(UI_SOURCE_FILES
        ${src_dir}/ui/common/componentSearchBox.cpp
        ${src_dir}/ui/common/diagramView.cpp
        ${src_dir}/ui/common/diagramScene.cpp
        ${src_dir}/ui/common/graphicalWire.cpp
        ${src_dir}/ui/common/graphicalComponent.cpp
        ${src_dir}/ui/common/icons.cpp
        ${src_dir}/ui/common/aboutDialog.cpp
        ${src_dir}/ui/logiFlow/components/graphicalLogicComponent.cpp
        ${src_dir}/ui/logiFlow/components/graphicalIO.cpp
        ${src_dir}/ui/logiFlow/components/graphicalGates.cpp
        ${src_dir}/ui/logiFlow/components/graphicalUtils.cpp
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
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile_commands.json")

if (CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
            ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif ()

if (MSVC)
    add_compile_options(/W4)
elseif (UNIX AND NOT APPLE)
    add_compile_options(-Wall -Wextra -Wno-unused-parameter -fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
elseif (WIN32 AND MINGW)
    add_compile_options(-Wall -Wextra -Wno-unused-parameter)
endif ()

find_package(Qt6 REQUIRED COMPONENTS Widgets SvgWidgets)
qt_standard_project_setup()

qt_add_executable(Silicon
        ${src_dir}/main.cpp)

target_sources(Silicon
        PRIVATE
        ${COMMON_SOURCE_FILES}
        ${EXTRA_COMPONENTS_SOURCE_FILES}
        ${UI_SOURCE_FILES})


target_compile_options(Silicon PRIVATE -Werror)
target_link_libraries(Silicon PRIVATE Qt6::Widgets Qt6::SvgWidgets)
enable_runtime_dll_copy_qt(Silicon)

file(GLOB image_resources CONFIGURE_DEPENDS "${resources_dir}/*.jpg" "${resources_dir}/*.png")
file(GLOB font_resources CONFIGURE_DEPENDS "${resources_dir}/fonts/*.ttf" "${resources_dir}/fonts/*.otf")
file(GLOB licenses_file CONFIGURE_DEPENDS "${resources_dir}/licenses/*.txt")

file(GLOB app_icons CONFIGURE_DEPENDS "${resources_dir}/*.svg")
file(GLOB ui_icons CONFIGURE_DEPENDS "${resources_dir}/icons/*.svg")
file(GLOB gate_icons CONFIGURE_DEPENDS "${resources_dir}/gates/*.svg")
file(GLOB component_icons CONFIGURE_DEPENDS "${resources_dir}/other_components/*.svg")

set(all_icons ${ui_icons} ${gate_icons} ${component_icons})

qt_add_resources(Silicon "images" BASE ${resources_dir}
        FILES ${image_resources})

qt_add_resources(Silicon "fonts" BASE ${resources_dir}
        FILES ${font_resources})

qt_add_resources(Silicon "icons" BASE ${resources_dir}
        FILES ${all_icons})


qt_add_resources(Silicon "licenses" BASE ${resources_dir}
        FILES ${licenses_file})


# Copy the DLLs if on Windows
if (WIN32)
    set(VCPKG_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
    set(OUTPUT_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

    if (NOT EXISTS "${VCPKG_BIN_DIR}")
        message(FATAL_ERROR "${VCPKG_BIN_DIR} does not exist, cannot proceed!")
    endif ()

    if (NOT EXISTS "${OUTPUT_BIN_DIR}")
        file(MAKE_DIRECTORY "${OUTPUT_BIN_DIR}")
    endif ()

    if (EXISTS "${VCPKG_BIN_DIR}")
        message(STATUS "Copying DLLs from vcpkg: ${VCPKG_BIN_DIR} -> ${OUTPUT_BIN_DIR}")
        file(GLOB VCPKG_DLLS "${VCPKG_BIN_DIR}/*.dll")
        file(COPY ${VCPKG_DLLS} DESTINATION "${OUTPUT_BIN_DIR}")
    endif ()
endif ()

if (SILICON_BUILD_TESTS)
    include(GoogleTest)
    find_package(GTest REQUIRED)

    if (WIN32)
        set(GTEST_LINKED_AS_SHARED_LIBRARY ON)
    endif ()

    add_subdirectory("${CMAKE_SOURCE_DIR}/tests")
    enable_testing()
endif ()
