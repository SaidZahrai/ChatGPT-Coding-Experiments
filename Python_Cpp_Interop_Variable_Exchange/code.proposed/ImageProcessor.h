#include <opencv2/opencv.hpp> // Not given by ChatGPT 4

class ImageProcessor {
public:
    cv::Mat process(cv::Mat img) {
        cv::Mat gray, small;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
        cv::pyrDown(gray, small);
        return small;
    }
};
