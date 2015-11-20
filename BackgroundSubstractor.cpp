/*
高斯混合背景建模
*/
#include "opencv.hpp"
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
		VideoCapture video(0);
		Mat frame, front;
		int frameNum = 1;
		BackgroundSubtractorMOG2 bgSubtractor(20, 16, true);
		while (1){
			if (!video.read(frame))
				break;
			video.read(frame);
			++frameNum;
			bgSubtractor(frame, front, 0.001);
			erode(front, front, cv::Mat());
			dilate(front, front, cv::Mat());
			if (!frame.empty()){
				imshow("Src", frame);
				imshow("Front", front);
				//cout << frameNum << endl;
			}
			waitKey(10);  
		}
    return 0;
}