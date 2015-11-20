#include <iostream>
#include <opencv.hpp>
#include "c:\opencv\build\include\opencv2\imgproc\types_c.h"
#include "c:\opencv\build\include\opencv2\imgcodecs\imgcodecs_c.h" 
using namespace cv;
using namespace std;
void topng(Mat &src)
{
	for (int x = 0; x < src.rows; x++)
		for (int y = 0; y < src.cols; y++)
		{
		Vec4b & rgba = src.at<Vec4b>(x, y);
		//将背景区域变为透明色		
		if (rgba[0] >= 250)
			rgba[3] = 0;
		}
}
int main()
{	
	Mat Frame = imread("C:\\Users\\PC\\Desktop\\1.png");
	imshow("src", Frame);
	cvtColor(Frame, Frame, CV_BGR2BGRA);
	topng(Frame);
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	imshow("png", Frame);
	imwrite("C:\\Users\\PC\\Desktop\\_1_.png", Frame, compression_params);
	waitKey(20);
	getchar();
	return 0;
}