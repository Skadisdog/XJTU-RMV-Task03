#include "target.hpp"

using namespace std;

namespace TARGET
{
    cv::Mat Target::removeAnnulus(const cv::Mat& src, const cv::Point& center, int innerRadius, int outerRadius)
    {
        // 创建一个与源图像大小相同的掩膜，初始化为全黑
        cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);

        // 在掩膜上绘制一个实心的白色圆（外圆）
        cv::circle(mask, center, outerRadius, cv::Scalar(255), -1);

        // 在掩膜上绘制一个实心的黑色圆（内圆），以创建一个环形区域
        cv::circle(mask, center, innerRadius, cv::Scalar(0), -1);

        // 创建一个结果图像的副本
        cv::Mat result = src.clone();

        // 使用掩膜将圆环区域中的像素设置为黑色
        result.setTo(cv::Scalar(0, 0, 0), mask);

        return result;
    }
    void Target::firstFind(cv::Mat &srcin) // 第一帧R
    {
        cv::Mat src = srcin.clone();
        std::vector<std::vector<cv::Point>> contours = findCountor(src);

        // 过滤最小面积轮廓
        int RNum=-1;
        double RArea=numeric_limits<double>::max();
        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            if(area < RArea)
            {
                RArea = area;
                RNum = i;
            }
        }
        // 查找R的中心点
        cv::RotatedRect RminRect = cv::minAreaRect(contours[RNum]);
        /* 调试
        cv::Point2f rect_points[4];
        RminRect.points(rect_points);
        for (int i = 0; i < 4; i++)
        {
            cv::line(src, rect_points[i], rect_points[(i+1)%4], cv::Scalar(0, 255, 0), 2);
        }
        cv::imshow("src", src);
        cv::waitKey(1);
        */
        RCenter = RminRect.center;
    }
    std::vector<std::vector<cv::Point>> Target::findCountor(const cv::Mat srcin)
    {

        cv::Mat grayImage;
        cv::Mat binaryImage;
        cv::Mat src = srcin.clone();
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
        return std::vector<std::vector<cv::Point>>(contours);
    }
    cv::Mat Target::findTarget(const cv::Mat srcin) // 找锤子头
    {
        cv::Mat src = srcin.clone();
        std::vector<std::vector<cv::Point>> contours = findCountor(src);
        return src;
    }
    cv::Mat Target::findR(const cv::Mat srcin)
    {
        cv::Mat onlyRImage;
        cv::Mat src = srcin.clone();
        if(flag == 0)// 找到第一帧的R
        {
            flag=1;
            firstFind(src);
        }
        cv::circle(src, RCenter, 145, cv::Scalar(0, 255, 255), 1);
        onlyRImage = removeAnnulus(src, RCenter, 12, 500); // 删除R外面的所有东西
        std::vector<std::vector<cv::Point>> contours = findCountor(onlyRImage);
        // 获取中心点
        cv::RotatedRect RminRect = cv::minAreaRect(contours[0]);
        RCenter = RminRect.center;
        cv::waitKey(1);
        cv::drawContours(src, contours, -1, cv::Scalar(0, 255, 0), 1);
        return src;
    }
}