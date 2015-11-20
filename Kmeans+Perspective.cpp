#include "opencv2/opencv.hpp"
#include "common.h"
#include "LineFinder.h"
/*
1 概率hough变换检测直线
2 计算直线交点
3 Kmeans聚类识别角点
4 透视变换矫正
*/
using namespace cv;
using namespace std;

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b);
void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center);
vector <Point2f> Kmeans(vector <Point2f> src,int clusterCount);
int main(int argc, char *argv[])
{
	cv::Mat image = cv::imread ("test (4).jpg");
	cv::Mat gray;
	Mat Frame;
	Frame = image.clone();
	cv::cvtColor (image,gray,CV_BGRA2GRAY);

	//Canny边缘检测
	//cv::adaptiveThreshold(gray,gray,255,CV_ADAPTIVE_THRESH_MEAN_C ,CV_THRESH_BINARY_INV,11,7);
	//cv::threshold(gray,gray,0,255,CV_THRESH_BINARY_INV+CV_THRESH_OTSU);

	cv::Canny (gray,gray,100,150,3);
	imshow ("",gray);
	//概率Hough直线检测
	vector<Vec4i>lines;
	cv::HoughLinesP(gray,lines,1,CV_PI/180,70,100,10);

	//绘制直线	
	for(int i = 0;i < lines.size();i++)
	{
	//cout << i <<endl;
	line(image,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,0),2,8,0);
	}
	std::vector<cv::Point2f> corners;
	for (int i = 0; i < (int)lines.size(); i++)
	{
		for (int j = i+1; j < (int)lines.size(); j++)
		{
			cv::Point2f pt = computeIntersect(lines[i], lines[j]);
			if (pt.x >= 0 && pt.y >= 0 &&pt.x <image.cols&&pt.y<image.rows)
				corners.push_back(pt);
		}
	}	for(int i =0;i<(int)corners.size();i++)
	{
		cv::circle(image,corners[i],2,Scalar(0,255,0),2,8,0);
	}
	
	//这里使用kmeans聚类算法对点集分类，并获取四个聚类中心作为最终需要的点。

	if(corners.size() > 4){
	vector <Point2f> result;
	
	result = Kmeans(corners,4);
	corners.clear();
	corners = result;
	//绘制顶点
		for(int i =0;i<(int)result.size();i++)
	{
		cv::circle(image,result[i],4,Scalar(0,0,255),3,8,0);
	}
	//多边形逼近，判断是否是四边形
		
	std::vector<cv::Point2f> approx;
	cv::approxPolyDP(cv::Mat(corners), approx,cv::arcLength(cv::Mat(corners), true) * 0.02, true);

	if (approx.size() != 4)
	{
	std::cout << "The object is not quadrilateral!" << std::endl;
	//return -1;
	}
	//得到四边形质心
	cv::Point2f center(0,0);
	for (int i = 0; i < corners.size(); i++)
	center += corners[i];
	center *= (1. / corners.size());
	sortCorners(corners, center);
	//Now the corners is perfectly sorted i.e. corners[0] = top-left, corners[1] = top-right, corners[2] = bottom-right, and corners[3] = bottom-left.
	//定义目标图像????????????
	cv::Mat quad = cv::Mat::zeros(480, 640, CV_8UC3);
	//目标图像的四个顶点
	std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	quad_pts.push_back(cv::Point2f(0, quad.rows));
	// 得到透视变换矩阵
	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
	// Apply perspective transformation
	cv::warpPerspective(Frame, quad, transmtx, quad.size());
	cv::imshow("quadrilateral", quad);
	
	
	}
	
	cv::imshow("Result",image);
	cv::waitKey (0);
}







cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4)))
	{
		cv::Point2f pt;
		pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;
		pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d;
		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;

	for (int i = 0; i < (int)corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}

	cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
	cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
	cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
	cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}


vector <Point2f> Kmeans(vector <Point2f> src,int clusterCount)
{
	vector <Point2f> result;
	Mat samples(src.size(),2,CV_32F);
	//put points into samples
	for (int i = 0; i < samples.rows; ++i)
	{
		samples.at<float>(i,0) = src[i].x;
		samples.at<float>(i,1) = src[i].y;
	}
	Mat labels;
	int attempts = 20;//5 loops 
	Mat centers;

	kmeans(samples, clusterCount, labels,
		TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
		attempts, KMEANS_PP_CENTERS, centers);
	result.clear();
	for( int i =0;i<4;i++){
		Point pt;
		pt.x = (int)centers.at<float>(i,0);
		pt.y = (int)centers.at<float>(i,1);
		result.push_back(pt);
	}
	return result;
}