#include <opencv2/opencv.hpp> // Not given by ChatGPT 4

class ImageWriter {
public:
    void write(const std::string& filename, cv::Mat img) {
        cv::imwrite(filename, img);
    }
};
