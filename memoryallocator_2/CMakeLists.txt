cmake_minimum_required(VERSION 3.10)

project(MemoryManagerTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MemoryManagerTest main.cpp MemoryManager.h)

target_link_libraries(MemoryManagerTest pthread)
