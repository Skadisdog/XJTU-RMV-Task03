#include "windmill.hpp"
#include "target.hpp"

using namespace std;
using namespace cv;

int main()
{
    std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    WINDMILL::WindMill wm(t.count());
    TARGET::Target tg;
    cv::Mat src;
    cv::Mat outputImage;
    cv::Point2d RCenter,targetCenter;


    // 创建窗口并调整窗口大小
    std::string windowName1 = "window1";
    cv::namedWindow(windowName1, cv::WINDOW_NORMAL); // 允许调整窗口大小
    cv::resizeWindow(windowName1, 800, 600); // 设置窗口大小为 800x600


    while (1)
    {
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        src = wm.getMat((double)t.count()/1000);
        
        //==========================代码区========================//
        
        outputImage = tg.findR(src);
        cv::imshow(windowName1, outputImage);

        //=======================================================//
        
        waitKey(1);
    }
}