#include <opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
Mat image = imread("C:\\Users\\Tong\\Desktop\\Car\\001.jpg");
Mat gray;
cvtColor(image, gray, COLOR_RGB2GRAY);
Canny(gray, gray, 100, 150, 3);
vector<Vec4i>lines;
HoughLinesP(gray, lines, 1, CV_PI / 180, 70, 100, 10);
for (int i = 0; i < lines.size(); i++)
{
line(image, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(36, 28, 238), 2, 8, 0);
}
vector<Point2f>corners;
for (int i = 0; i < (int)lines.size(); i++)
{
for (int j = i + 1; j < (int)lines.size(); j++)
{
cv::Point2f pt = computeIntersect(lines[i], lines[j]);
if (pt.x >= 0 && pt.y >= 0 && pt.x <image.cols&&pt.y<image.rows)
corners.push_back(pt);
}
}
//corners = Kmeans(corners, 4);
for (int i = 0; i<(int)corners.size(); i++)
{
cv::circle(image, corners[i], 2, Scalar(0, 255, 0), 2, 8, 0);
}
imshow("", image);int main(int argc, char *argv[])
{
	Mat image, hsv;

	waitKey(0);
}
}
