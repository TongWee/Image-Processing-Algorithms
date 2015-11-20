#include "c:\opencv\build\include\opencv2\imgproc\types_c.h"
#include "opencv.hpp"
#define PI 3.14
#define CVX_RED Scalar(0xff,0x00,0x00)
#define CVX_BLUE Scalar(0x00,0x00,0xff)
#define CVX_YELLOW Scalar(0xff,0xff,0x00)
#define CVX_GREEN Scalar(143,195,31)
#define CVX_WHITE Scalar(0xff,0xff,0xff)
#define CVX_BLACK Scalar(0x00,0x00,0x00)
#define CVX_RANDOM Scalar(rand()%255,rand()%255,rand()%255)

using namespace cv;
using namespace std;

Point center(vector<Point> contour);
double circle_degree(vector<Point> contour,Point center);
int main(int argc, char** argv)
{
	Mat img,dst;
	VideoCapture capture;
	//capture.open(0);
	capture.open("1.avi");
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	while (1)
	{
		if (!capture.read(img))
			break;
		vector<Rect> found;
		cvtColor(img, dst, CV_BGR2GRAY);
		hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
		//imshow("ddd", dst);
		for (int i = 0; i < found.size(); i++)
		{
			rectangle(img, found[i], CVX_RANDOM, 2, 8, 0);
		}
		imshow("", img);
		waitKey(10);
	}
	return 0;
}
Point center(vector<Point> contour)
{
	Point center(0,0);
	double _x = 0,_y = 0,count;
	count = contour.size();
	for (int i = 0; i < contour.size(); i++)
	{
		_x += contour[i].x;
		_y += contour[i].y;
	}
	center.x = _x / count;
	center.y = _y / count;
	return center;
}
double circle_degree(vector<Point> contour,Point center)
{
	double res,rowmom=0,colmom=0;
	for (int i = 0; i < contour.size(); i++)
	{
		rowmom += (contour[i].x - center.x)*(contour[i].x - center.x);
		colmom += (contour[i].y - center.y)*(contour[i].y - center.y);
	}
	rowmom = rowmom / contour.size();
	colmom = colmom / contour.size();
	res = rowmom / colmom;
	return res;
}

