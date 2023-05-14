#include "ImageReader.h"

void ImageReader::read(const std::string& filename, ImageDataHolder& dataHolder) {
    dataHolder.img = cv::imread(filename, cv::IMREAD_COLOR);
    if(dataHolder.img.empty())
        throw std::runtime_error("Could not open or find the image");
}
