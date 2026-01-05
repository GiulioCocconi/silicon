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


# Windows-specific configuration

function(win_deploy_qt target)
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

if (WIN32 AND SILICON_USE_VCPKG)
    # Copy the DLLs

    if (VCPKG_BUILD_TYPE STREQUAL "debug")
        set(VCPKG_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin")
    else ()
        set(VCPKG_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
    endif ()

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
