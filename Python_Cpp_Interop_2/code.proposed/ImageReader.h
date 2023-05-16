#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "ImageDataHolder.h"

class ImageReader {
public:
    void read(const std::string& filename, ImageDataHolder& dataHolder);
};
