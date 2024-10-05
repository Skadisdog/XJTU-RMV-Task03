#include "windmill.hpp"
#include "target.hpp"
#include "fitting.hpp"
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main()
{
    std::ofstream outputFile("output.txt");
    std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    WINDMILL::WindMill wm(t.count()/1000.0);
    TARGET::Target tg;
    cv::Mat src;
    int count = 0;
    double distance = 0;
    int firstFlag = 1;
    cv::Point2f RCenterLast, targetCenterLast;
    std::chrono::milliseconds tLast;
    FITTING::Fitting ft;
    Parameter a;

    while(1) // 距离 169.4
    {
        count++;
        t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        src = wm.getMat((double)t.count()/1000.0);
        tg.findR(src);
        tg.findTarget(src);
        cv::Point2f RCenter = tg.getR();
        cv::Point2f targetCenter = tg.getTarget();
        /*
        cv::circle(src, targetCenter, 5, cv::Scalar(255,0,0));
        cv::circle(src, targetCenterLast, 5, cv::Scalar(255,0,0));
        cv::imshow("1", src);
        cv::waitKey(1);
        */
        if(firstFlag == 1)
        {
            firstFlag = 0;
            RCenterLast = RCenter;
            targetCenterLast = targetCenter;
            tLast = t;
            continue;
        }
        double dt = ((double)t.count()-(double)tLast.count())/1000.0;
        double distance = cv::norm(targetCenterLast - targetCenter);
        double drad = distance / cv::norm(RCenter - targetCenter);
        if(count % 5 == 0)
        {
            ft.addData({(double)t.count()/1000.0, drad/dt});
            outputFile<<fixed<<(double)t.count()/1000.0<<" "<<drad/dt<<endl;
        }
        if(count % 100 == 0)
        {
            a = ft.calculate();
            std::cout<<fixed<<a.A<<" "<<a.omega<<" "<<a.phi<<" "<<a.B<<std::endl;
            if(abs(a.A-0.785)<=0.785*0.05 &&
                abs(a.omega-1.884)<=1.884*0.05 &&
                abs(a.phi-0.24)<=0.24*0.05 &&
                abs(a.B-1.305)<=1.305*0.05)
            {
                cout<<count<<endl;
                break;
            }
            //ft.clearData();
            //break;
        }
        tLast = t;
        RCenterLast = RCenter;
        targetCenterLast = targetCenter;
        /*circle(src, targetCenter, 3, Scalar(255,0,0), -1);
        cv::imshow("1", src);*/
        cv::waitKey(10);
    }
    return 0;
}