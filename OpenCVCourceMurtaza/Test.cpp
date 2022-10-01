#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	string path = "C:\\Users\\jason\\OneDrive\\Desktop\\GOPR0786.JPG";
	Mat img = imread(path);
	imshow("Image", img);
	waitKey(0);

	return 0;
}