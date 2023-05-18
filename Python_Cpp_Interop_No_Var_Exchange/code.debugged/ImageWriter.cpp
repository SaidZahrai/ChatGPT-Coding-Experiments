#include "ImageWriter.h"

void ImageWriter::write(const std::string& filename, ImageDataHolder& dataHolder) {
    cv::imwrite(filename, dataHolder.img);
}
