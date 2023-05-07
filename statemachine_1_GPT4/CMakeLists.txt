# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.8)

# Set the project name and version
project(StateMachineProject VERSION 1.0)

# Use C++17 for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories for header files
include_directories(include)

# Add the library with source files
file(GLOB SOURCES "src/*.cpp")
add_library(StateMachineLib ${SOURCES})

# Add the test executable
add_executable(TestStateMachine application/test.cpp)

# Link the test executable with the library
target_link_libraries(TestStateMachine StateMachineLib)
