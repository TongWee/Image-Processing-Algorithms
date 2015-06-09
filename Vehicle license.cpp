#include <opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
	Mat image,frame;
	Mat hsv[3];
	VideoCapture capture(0);
	while (capture.read(frame)){
		
		cvtColor(frame, image,COLOR_RGB2HSV);
		split(image, hsv);
		inRange(hsv[0], Scalar(15, 0, 0), Scalar(40, 0, 0), hsv[0]);
		inRange(hsv[1], Scalar(0.35 * 255, 0, 0), Scalar(255, 0, 0), hsv[1]);
		inRange(hsv[2], Scalar(0.35 * 255, 0, 0), Scalar(255, 0, 0), hsv[2]);
		imshow("h", hsv[0]);
		imshow("s", hsv[1]);
		imshow("v", hsv[2]);
		
		waitKey(10);
	}
}

Mat colorMatch(const Mat& src, Mat& match, const int r, const bool adaptive_minsv)
{
	// S和V的最小值由adaptive_minsv这个bool值判断
	// 如果为true，则最小值取决于H值，按比例衰减
	// 如果为false，则不再自适应，使用固定的最小值minabs_sv
	// 默认为false
	const float max_sv = 255;
	const float minref_sv = 64;

	const float minabs_sv = 95;

	//blue的H范围
	const int min_blue = 100;  //100
	const int max_blue = 140;  //140

	//yellow的H范围
	const int min_yellow = 15; //15
	const int max_yellow = 40; //40

	Mat src_hsv;
	// 转到HSV空间进行处理，颜色搜索主要使用的是H分量进行蓝色与黄色的匹配工作
	cvtColor(src, src_hsv, COLOR_RGB2HSV);

	vector<Mat> hsvSplit;
	split(src_hsv, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, src_hsv);

	//匹配模板基色,切换以查找想要的基色
	int min_h = 0;
	int max_h = 0;
	switch (r) {
	case 0:
		min_h = min_blue;
		max_h = max_blue;
		break;
	case 1:
		min_h = min_yellow;
		max_h = max_yellow;
		break;
	}

	float diff_h = float((max_h - min_h) / 2);
	int avg_h = min_h + diff_h;

	int channels = src_hsv.channels();
	int nRows = src_hsv.rows;
	//图像数据列需要考虑通道数的影响；
	int nCols = src_hsv.cols * channels;

	if (src_hsv.isContinuous())//连续存储的数据，按一行处理
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	float s_all = 0;
	float v_all = 0;
	float count = 0;
	for (i = 0; i < nRows; ++i)
	{
		p = src_hsv.ptr<uchar>(i);
		for (j = 0; j < nCols; j += 3)
		{
			int H = int(p[j]); //0-180
			int S = int(p[j + 1]);  //0-255
			int V = int(p[j + 2]);  //0-255

			s_all += S;
			v_all += V;
			count++;

			bool colorMatched = false;

			if (H > min_h && H < max_h)
			{
				int Hdiff = 0;
				if (H > avg_h)
					Hdiff = H - avg_h;
				else
					Hdiff = avg_h - H;

				float Hdiff_p = float(Hdiff) / diff_h;

				// S和V的最小值由adaptive_minsv这个bool值判断
				// 如果为true，则最小值取决于H值，按比例衰减
				// 如果为false，则不再自适应，使用固定的最小值minabs_sv
				float min_sv = 0;
				if (true == adaptive_minsv)
					min_sv = minref_sv - minref_sv / 2 * (1 - Hdiff_p); // inref_sv - minref_sv / 2 * (1 - Hdiff_p)
				else
					min_sv = minabs_sv; // add

				if ((S > min_sv && S < max_sv) && (V > min_sv && V < max_sv))
					colorMatched = true;
			}

			if (colorMatched == true) {
				p[j] = 0; p[j + 1] = 0; p[j + 2] = 255;
			}
			else {
				p[j] = 0; p[j + 1] = 0; p[j + 2] = 0;
			}
		}
	}

	//cout << "avg_s:" << s_all / count << endl;
	//cout << "avg_v:" << v_all / count << endl;

	// 获取颜色匹配后的二值灰度图
	Mat src_grey;
	vector<Mat> hsvSplit_done;
	split(src_hsv, hsvSplit_done);
	src_grey = hsvSplit_done[2];

	match = src_grey;

	return src_grey;
}