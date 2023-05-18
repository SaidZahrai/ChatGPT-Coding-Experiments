#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "ImageProcessor.h"
#include "ImageReader.h"
#include "ImageWriter.h"

PYBIND11_MODULE(libimageproc, m)
{
    pybind11::class_<ImageReader>(m, "ImageReader")
        .def(pybind11::init<>())
        .def("read", &ImageReader::read)
        .def_property_readonly("img", [](const ImageReader &self)
                               {
            cv::Mat img = self.img;
            pybind11::buffer_info buf_info(
                img.data,                                  /* Pointer to buffer */
                sizeof(unsigned char),                     /* Size of one scalar */
                pybind11::format_descriptor<unsigned char>::format(), /* Python struct-style format descriptor */
                3,                                         /* Number of dimensions */
                { img.rows, img.cols, img.channels() },    /* Buffer dimensions */
                { sizeof(unsigned char) * img.channels() * img.cols,  /* Strides (in bytes) for each index */
                sizeof(unsigned char) * img.channels(), 
                sizeof(unsigned char) }
            );
            return pybind11::array_t<unsigned char>(buf_info); });

    pybind11::class_<ImageProcessor>(m, "ImageProcessor")
        .def(pybind11::init<>())
        .def("process", [](ImageProcessor &self, pybind11::array_t<unsigned char> array)
             {
            pybind11::buffer_info buf_info = array.request();
            cv::Mat img(buf_info.shape[0], buf_info.shape[1], CV_8UC3, buf_info.ptr);
            cv::Mat result = self.process(img);
            pybind11::buffer_info result_buf_info(
                result.data,
                sizeof(unsigned char),
                pybind11::format_descriptor<unsigned char>::format(),
                3,
                { result.rows, result.cols, result.channels() },
                { sizeof(unsigned char) * result.channels() * result.cols, sizeof(unsigned char) * result.channels(), sizeof(unsigned char) }
            );
            return pybind11::array_t<unsigned char>(result_buf_info); });

    pybind11::class_<ImageWriter>(m, "ImageWriter")
        .def(pybind11::init<>())
        .def("write", [](ImageWriter &self, const std::string &filename, pybind11::array_t<unsigned char> array)
             {
            pybind11::buffer_info buf_info = array.request();
            cv::Mat img(buf_info.shape[0], buf_info.shape[1], CV_8UC3, buf_info.ptr);
            self.write(filename, img); });
}
