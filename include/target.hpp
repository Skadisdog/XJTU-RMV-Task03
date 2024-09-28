#ifndef TARGET_H_
#define TARGET_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <tuple>
#include <limits>

namespace TARGET
{
    class Target
    {
    private:
        cv::Mat grayImage;
        cv::Mat binaryImage;
    public:
        std::tuple<cv::Mat, cv::Point2d, cv::Point2d> findTarget(cv::Mat src);
    };
}

#endif