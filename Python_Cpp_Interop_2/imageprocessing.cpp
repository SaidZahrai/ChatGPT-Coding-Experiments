#include "ImageReader.h"
#include "ImageProcessor.h"
#include "ImageWriter.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(imageprocessing, m) {
    py::class_<ImageDataHolder>(m, "ImageDataHolder")
        .def(py::init<>());

    py::class_<ImageReader>(m, "ImageReader")
        .def(py::init<>())
        .def("read", &ImageReader::read);

    py::class_<ImageProcessor>(m, "ImageProcessor")
        .def(py::init<>())
        .def("process", &ImageProcessor::process);

    py::class_<ImageWriter>(m, "ImageWriter")
        .def(py::init<>())
        .def("write", &ImageWriter::write);
}
