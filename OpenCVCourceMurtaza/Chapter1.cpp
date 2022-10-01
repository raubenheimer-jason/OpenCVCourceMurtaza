#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Summary
/*
Images
Video
Webcam
*/

/*
// Images

void main()
{
	string path = "Resources/test.png";
	Mat img = imread(path);
	imshow("Image", img);
	waitKey(0);
}
*/


/*
// Video

void main()
{
	string path = "Resources/test_video.mp4";
	VideoCapture cap(path);
	Mat img;

	while (true)
	{
		cap.read(img);
		imshow("Image", img);
		waitKey(1); // 1ms
	}
}
*/


// Webcam

void main()
{
	VideoCapture cap(0); // imput camera id
	Mat img;

	while (true)
	{
		cap.read(img);
		imshow("Image", img);
		waitKey(1); // 1ms
	}
}