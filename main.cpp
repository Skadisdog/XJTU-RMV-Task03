#include "windmill.hpp"
#include "target.hpp"

using namespace std;
using namespace cv;

int main()
{
    std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    WINDMILL::WindMill wm(t.count()/1000);
    TARGET::Target tg;
    cv::Mat src;

    while (1)
    {
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        src = wm.getMat((double)t.count()/1000);
        
        tg.findR(src);
        tg.findTarget(src);
        cv::Point2f RCenter = tg.getR();
        cv::Point2f targetCenter = tg.getTarget();
        /*circle(src, targetCenter, 3, Scalar(255,0,0), -1);
        cv::imshow("1", src);
        cv::waitKey(1);*/
    }
    return 0;
}