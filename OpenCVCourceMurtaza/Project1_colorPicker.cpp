#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Colour detection


int main()
{
	//string path = "Resources/lambo.png"; // use shapes.png - good test
	//Mat img = imread(path);

	Mat mask;
	int hmin{}, smin{}, vmin{};
	int hmax{ 179 }, smax{ 255 }, vmax{ 255 };

	// How to find the values? -> create track bars
	namedWindow("trackbars", (640, 200));
	createTrackbar("Hue Min", "trackbars", &hmin, 179);
	createTrackbar("Hue Max", "trackbars", &hmax, 179);
	createTrackbar("Sat Min", "trackbars", &smin, 255);
	createTrackbar("Sat Max", "trackbars", &smax, 255);
	createTrackbar("Val Min", "trackbars", &vmin, 255);
	createTrackbar("Val Max", "trackbars", &vmax, 255);

	Mat img, imgHSV;
	VideoCapture cap(0); // imput camera id

	while (true) {
		// define limits
		Scalar lower(hmin, smin, vmin); // hue, saturation, value
		Scalar upper(hmax, smax, vmax);

		cap.read(img);

		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		//imshow("Image HSV", imgHSV);
		imshow("Image Mask", mask);

		waitKey(1);
	}

	return 0;
}

/*
Red:
hmin: 0
hmax: 179
smin: 76
smax: 196
vmin: 108
vmax: 166

Blue:
hmin: 97
hmax: 129
smin: 78
smax: 255
vmin: 90
vmax: 233
*/