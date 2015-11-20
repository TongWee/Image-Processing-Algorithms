#include <opencv.hpp>
using namespace std;
using namespace cv;
int diagram[500] = { 0 };
int radon_angle(int angle_range, int width_l, int width_h, int height_l, int height_h, Mat src, bool xy);
void main()
{
	int angle = 45;
	Mat src = imread("C:\\Users\\Tong\\Desktop\\Car\\011.jpg");
	resize(src, src, Size(400, 300));
	cvtColor(src, src, COLOR_RGB2GRAY);
	int vec_1, vec_2, hor_1, hor_2;
	vec_1 = radon_angle(angle, 0, src.rows, 0, src.cols, src,1);
	cout << vec_1 << endl;
	imshow("", src);
	waitKey(0);
}
///
///	0水平倾角 	1垂直倾角
///
int radon_angle(int angle_range, int width_l, int width_h, int height_l, int height_h, Mat src, bool xy){
	vector<int> diag;
	//Mat temp(300, 400, CV_8UC1, Scalar(255, 255, 255));
	imshow("src", src);
	int max_peak = 0, cur_angle = 0;
	double shift_x = 0;
	shift_x = src.rows*src.rows / (sqrt(src.rows*src.rows - src.cols*src.cols));
	Mat angles(300, 360, CV_8UC1, Scalar(255, 255, 255));
	for (int _angle = -angle_range; _angle <= angle_range; _angle++){
		memset(diagram, 0, sizeof(diagram));
		double angle;
			angle = _angle*3.14 / 180;
		int max = 0;
		for (int i = width_l; i < width_h; i++){
			for (int j = height_l; j < height_h; j++){				
				if (src.at<uchar>(i,j) == 0){
					double x=0, y=0;
					if (xy == true)
					{
						x = cos(angle)*i + sin(angle)*j;
						int ff;
						if (x<=0)
							ff = (int)(x + 0.5);
						else{
							ff = (int)(x - 0.5);
						}
						diagram[ff+180]++;
					}
					else
					{
						y = cos(angle)*i - sin(angle)*j;	
						diagram[(int)y]++;
					}
				}
			}
		}
		double temp_sum = 0;
		for (int i = 0; i < 500; i++){
			if (temp_sum < diagram[i])
				temp_sum = diagram[i];
		}
		Mat dia(500,500, CV_8UC1,Scalar(255,255,255));
		for (int i = 0; i < 500; i++){
			line(dia, Point(i, 500), Point(i, 500 - diagram[i]+1), Scalar(0, 0, 0), 1, 8, 0);
		}
		line(angles, Point(180 + _angle, 300), Point(180 + _angle, 300 * (1 - temp_sum / 1500)), Scalar(0, 0, 0), 2, 8, 0);
		cout << temp_sum <<"  --------------->   "<<_angle<<"°"<< endl;
		if (max_peak < temp_sum){
			max_peak = temp_sum;
			cur_angle = _angle;
		}		 
		imshow("直方图", dia);		
		imshow("", angles);		
		waitKey(0);  
	}
	return cur_angle;
}
