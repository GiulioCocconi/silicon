cmake_minimum_required(VERSION 3.26)

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

include_directories(${src_dir})

add_executable(logiFlow 
	${src_dir}/wire.cpp
	${src_dir}/gates.cpp
	${src_dir}/main.cpp)

target_compile_options(logiFlow PRIVATE -Wall -Wextra)
target_compile_features(logiFlow PRIVATE cxx_std_20)


find_package(fmt QUIET)

if(NOT fmt_FOUND)
	message(STATUS "fmt not found, fetching with FetchContent...")

	FetchContent_Declare(
		fmt
		GIT_REPOSITORY https://github.com/fmtlib/fmt.git
		GIT_TAG        10.2.1
	)
	FetchContent_MakeAvailable(fmt)

else()
	message(STATUS "FMT ALREADY INSTALLED!")
endif()

target_link_libraries(logiFlow PRIVATE fmt::fmt)


