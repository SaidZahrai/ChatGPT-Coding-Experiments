#pragma once
#include <string>
#include "ImageDataHolder.h"

class ImageWriter {
public:
    void write(const std::string& filename, ImageDataHolder& dataHolder);
};
