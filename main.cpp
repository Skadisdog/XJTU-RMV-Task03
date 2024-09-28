#include "windmill.hpp"

using namespace std;
using namespace cv;

int main()
{
    std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    WINDMILL::WindMill wm(t.count());
    cv::Mat src;
    cv::Mat grayImage;
    cv::Mat binaryImage;


    // 创建窗口并调整窗口大小
    std::string windowName1 = "window1";
    cv::namedWindow(windowName1, cv::WINDOW_NORMAL); // 允许调整窗口大小
    cv::resizeWindow(windowName1, 800, 600); // 设置窗口大小为 800x600
    std::string windowName2= "window2";
    cv::namedWindow(windowName2, cv::WINDOW_NORMAL); // 允许调整窗口大小
    cv::resizeWindow(windowName2, 800, 600); // 设置窗口大小为 800x600


    int frame = 0;
    while (1)
    {
        frame ++;
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        src = wm.getMat((double)t.count()/1000);
        
        //==========================代码区========================//
        
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
        // imshow("windmill", src);
        cv::imshow(windowName1, src);
        cv::imshow(windowName2, binaryImage);

        //=======================================================//
        
        waitKey(1);
    }
}