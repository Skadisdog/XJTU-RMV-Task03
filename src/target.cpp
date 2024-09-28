#include "target.hpp"

using namespace std;

namespace TARGET
{
    std::tuple<cv::Mat, cv::Point2d, cv::Point2d> Target::findTarget(cv::Mat src)
    {
        // 转换为灰度图像
        cv::cvtColor(src, grayImage, cv::COLOR_BGR2GRAY);

        // 应用阈值处理
        cv::threshold(grayImage, binaryImage, 1, 255, cv::THRESH_BINARY);


        // 创建结构元素
        int morph_size = 5; // 核大小，可以根据需要调整
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                    cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                                                    cv::Point(morph_size, morph_size));

        cv::dilate(binaryImage, binaryImage, element);
        cv::erode(binaryImage, binaryImage, element);

        // 查找轮廓
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(binaryImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


        // 过滤两个最小面积轮廓
        int RNum=-1, targetNum=-1;
        double RArea=numeric_limits<double>::max(), targetArea=numeric_limits<double>::max();
        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            if(area < RArea)
            {
                targetArea = RArea;
                RArea = area;
                targetNum = RNum;
                RNum = i;
            }
            else if(area < targetArea)
            {
                targetArea = area;
                targetNum = i;
            }
        }
        cv::drawContours(src, contours, RNum, cv::Scalar(0, 255, 0), 1);
        cv::drawContours(src, contours, targetNum, cv::Scalar(0, 255, 0), 1);
        return std::make_tuple(src, cv::Point2d(1,1), cv::Point2d(1,1));
    }
}