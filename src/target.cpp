#include "target.hpp"

using namespace std;

namespace TARGET
{
    cv::Mat Target::removeAnnulus(const cv::Mat& src, const cv::Point& center, int innerRadius, int outerRadius) // 掩模遮盖
    {
        // 创建一个与源图像大小相同的掩膜，初始化为全黑
        cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);

        // 在掩膜上绘制一个实心的白色圆（外圆）
        if(outerRadius != 0)
            cv::circle(mask, center, outerRadius, cv::Scalar(255), -1);

        // 在掩膜上绘制一个实心的黑色圆（内圆），以创建一个环形区域
        if(innerRadius != 0)
            cv::circle(mask, center, innerRadius, cv::Scalar(0), -1);

        // 创建一个结果图像的副本
        cv::Mat result = src.clone();

        // 使用掩膜将圆环区域中的像素设置为黑色
        result.setTo(cv::Scalar(0, 0, 0), mask);

        return result;
    }
    void Target::firstFind(const cv::Mat &srcin) // 第一帧R
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
        RCenter = RminRect.center;
    }
    void Target::firstFindTarget(const cv::Mat &srcin) // 找第一帧的目标
    {
        cv::Mat src = srcin.clone();
        cv::Mat noRimage = removeAnnulus(src, RCenter, 0, 145);
        std::vector<std::vector<cv::Point>> contours = findCountor(noRimage);

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
        // 查找锤头的中心点
        cv::RotatedRect targetminRect = cv::minAreaRect(contours[RNum]);
        targetCenter = targetminRect.center;
    }
    std::vector<std::vector<cv::Point>> Target::findCountor(const cv::Mat srcin) // 找轮廓
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
    void Target::findTarget(const cv::Mat srcin) // 找锤子头
    {
        cv::Mat src = srcin.clone();
        if(tarFlag == 0)
        {
            tarFlag = 1;
            firstFindTarget(src);
        }
        cv::Mat noRimage = removeAnnulus(src, RCenter, 0, 145);
        cv::Mat onlyHead = removeAnnulus(noRimage, targetCenter, 70, 400);
        std::vector<std::vector<cv::Point>> contours = findCountor(onlyHead);
        cv::RotatedRect targetMinRect = cv::minAreaRect(contours[0]);
        targetCenter = targetMinRect.center;
        return ;
    }
    void Target::findR(const cv::Mat srcin) // 找R
    {
        cv::Mat onlyRImage;
        cv::Mat src = srcin.clone();
        if(flag == 0)// 找到第一帧的R
        {
            flag=1;
            firstFind(src);
        }
        onlyRImage = removeAnnulus(src, RCenter, 12, 250); // 删除R外面的所有东西
        std::vector<std::vector<cv::Point>> contours = findCountor(onlyRImage);
        // 获取中心点
        cv::RotatedRect RminRect = cv::minAreaRect(contours[0]);
        RCenter = RminRect.center;
        return ;
    }
    cv::Point2f Target::getR()
    {
        return RCenter;
    }
    cv::Point2f Target::getTarget()
    {
        return targetCenter;
    }
}