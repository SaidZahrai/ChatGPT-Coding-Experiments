cmake_minimum_required(VERSION 3.14)  # FetchContent is available in 3.14+
project(imageprocessing)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.7.1
)
FetchContent_GetProperties(pybind11)
if(NOT pybind11_POPULATED)  # Have we downloaded pybind11 yet?
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()

add_library(imageprocessing MODULE imageprocessing.cpp ImageReader.cpp ImageProcessor.cpp ImageWriter.cpp)
target_link_libraries(imageprocessing PRIVATE pybind11::module ${OpenCV_LIBS})
