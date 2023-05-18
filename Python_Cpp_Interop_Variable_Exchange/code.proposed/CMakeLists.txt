cmake_minimum_required(VERSION 3.10)
project(imageproc)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(pybind11 REQUIRED)

add_library(imageproc MODULE imageproc.cpp)
target_link_libraries(imageproc PRIVATE ${OpenCV_LIBS} pybind11::module)
