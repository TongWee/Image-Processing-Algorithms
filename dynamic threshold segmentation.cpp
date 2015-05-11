#include <iostream>
#include <opencv.hpp>
#include "c:\opencv\build\include\opencv2\imgproc\types_c.h"
#include "c:\opencv\build\include\opencv2\imgcodecs\imgcodecs_c.h" 
#define DIR "C:\\Users\\Tong\\Desktop\\test\\3333.jpg"
using namespace cv;
using namespace std;

void CreateTree(Mat &src, Rect roi);
double AveGray(const Mat src, Rect roi);
int main()
{
	Mat src = imread(DIR,CV_LOAD_IMAGE_GRAYSCALE);
	Rect roi = Rect(0, 0, src.cols, src.rows);
	CreateTree(src, roi);
	imshow("111", src);
	waitKey(0);
	return 0;
}

void CreateTree(Mat &src, Rect roi)
{
	if (AveGray(src,roi)<130&&roi.width*roi.height>1000)
	{
		int x = roi.x, y = roi.y;
		int width = roi.width / 2, height = roi.height / 2;
		Rect LU = Rect(x, y, width,height);
		Rect RU = Rect(x+width, y, width, height); 
		Rect LD = Rect(x, y+height, width, height); 
		Rect RD = Rect(x+width, y+height, width, height);
		rectangle(src, LU, Scalar(0, 0, 0), 2, 8, 0);
		rectangle(src, RU, Scalar(0, 0, 0), 2, 8, 0);
		rectangle(src, LD, Scalar(0, 0, 0), 2, 8, 0);
		rectangle(src, RD, Scalar(0, 0, 0), 2, 8, 0);
		imshow("111", src);
		waitKey(0);
		CreateTree(src,LU);//LU
		CreateTree(src, RU);//RU
		CreateTree(src, LD);//LD
		CreateTree(src, RD);//RD
	}
	else
	{
		//Mat temp;
		//threshold(src(roi),temp,0,255, CV_THRESH_OTSU | CV_THRESH_BINARY);
		//temp.copyTo(src(roi), temp);
		//rectangle(src, roi, Scalar(0, 0, 0), 2, 8, 0);
		circle(src, Point(roi.x + roi.width / 2, roi.y + roi.height / 2),1,Scalar(255,255,255),2,8,0);
	
	}
}
double AveGray(const Mat src, Rect roi)
{
	double sum = 0;
	uchar ptr = 0;
	//cout << "IMG = " << src.rows << "---" << src.cols << endl;
	//cout << "ROI = " << roi.height << "---" << roi.width<< endl;
	//cout << "Start: " << roi.x<<"----"<<roi.y << endl;	
	for (int x = roi.x; x < roi.x+roi.width-1; x++)
		for (int y = roi.y; y < roi.y+roi.height-1; y++)
		{
 		ptr = src.at<char>(y, x);
		sum += ptr;
		}
	return sum/(roi.width*roi.height);
}