/*
CamShift算法流程：
Step 1：将整个图像设为搜寻区域。
Step 2：初始话Search Window的大小和位置。
Step 3：计算Search Window内的彩色概率分布，此区域的大小比Search Window要稍微大一点。
Step 4：运行MeanShift。获得Search Window新的位置和大小。
Step 5：在下一帧视频图像中，用Step 3获得的值初始化Search Window的位置和大小。跳转到Step 3继续运行。
*/
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

Mat image;
Rect selection;

static void onMouse( int event, int x, int y, int, void* ){...}
int main( int argc, const char** argv )
{
    Mat hue;
    int ch[] = {0, 0};
    cvtColor(image, hsv, COLOR_BGR2HSV);
    if( trackObject )
    {
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
        if( trackObject < 0 )
        {
            //生成直方图
            calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
            //原数组，数组位数，通道，掩膜，输出直方图，直方图维数，x轴，y轴
            
        }
        //反向投影
        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        RotatedRect trackBox = CamShift(backproj, trackWindow,
                            TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
        //CV_TERMCRIT_EPS是通过forest_accuracy方法,
        //CV_TERMCRIT_ITER是通过max_num_of_trees_in_the_forest方法    
        
        if( trackWindow.area() <= 1 )
        {
            int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
            trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                               trackWindow.x + r, trackWindow.y + r) &
                          Rect(0, 0, cols, rows);
        }

        if( backprojMode )
            cvtColor( backproj, image, COLOR_GRAY2BGR );
        ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
    }
    }
    return 0;
}
