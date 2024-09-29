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
        cv::Point2f targetCenter;
        cv::Point2f RCenter;
        int flag = 0;
        cv::Mat removeAnnulus(const cv::Mat& src, const cv::Point& center, int innerRadius, int outerRadius);
        void firstFind(cv::Mat &srcin);
        std::vector<std::vector<cv::Point>> findCountor(const cv::Mat src);
    public:
        cv::Mat findTarget(const cv::Mat srcin);
        cv::Mat findR(const cv::Mat srcin);
    };
}

#endif