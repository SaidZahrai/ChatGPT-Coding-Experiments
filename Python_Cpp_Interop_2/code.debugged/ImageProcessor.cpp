#include "ImageProcessor.h"

void ImageProcessor::process(ImageDataHolder& dataHolder) {
    cv::resize(dataHolder.img, dataHolder.img, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
    cv::cvtColor(dataHolder.img, dataHolder.img, cv::COLOR_BGR2GRAY);
}
