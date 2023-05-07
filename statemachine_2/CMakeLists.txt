# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyProject VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory to the project
include_directories(include)

# Add the source files to the project
add_library(src
    src/state_base.cpp
    src/machine_base.cpp
    src/derived_states.cpp
    src/derived_machine.cpp
)

# Add the executable target for the test program
add_executable(application
    application/test.cpp
)

# Link the source files to the executable target
target_link_libraries(application src)
