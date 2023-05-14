#include <opencv2/opencv.hpp>
#include <string>

class ImageReader {
public:
    cv::Mat img;

    void read(const std::string& filename) {
        img = cv::imread(filename, cv::IMREAD_COLOR);
        if (img.empty())
            throw std::invalid_argument("Could not open or find the image");
    }
};

