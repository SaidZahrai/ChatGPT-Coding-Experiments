# cmake_minimum_required(VERSION 3.10)

# project(ThreadSafeMemoryPool)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# add_executable(ThreadSafeMemoryPool main.cpp)


cmake_minimum_required(VERSION 3.16)

project(ThreadSafeMemoryPoolExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_executable(ThreadSafeMemoryPoolExample main.cpp)

target_link_libraries(ThreadSafeMemoryPoolExample PRIVATE Threads::Threads)
