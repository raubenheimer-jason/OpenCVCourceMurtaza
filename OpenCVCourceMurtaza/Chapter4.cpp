#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Draw shapes and text

void main()
{
	// Blank image
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255)); // 8 bits (0-255) unsigned 3 colour chanels, Scalar for colour

	//circle(img, Point(256, 256), 155, Scalar(0, 69, 255), 10); // 10 is thickness
	circle(img, Point(256, 256), 155, Scalar(0, 69, 255), FILLED);

	rectangle(img, Point(130, 226), Point(382, 286), Scalar(255, 255, 255), FILLED); // Point(x,y), Point(x1,y1)

	line(img, Point(130, 296), Point(382, 296), Scalar(255, 255, 255), 2); // start and end point

	putText(img, "OpenCV Course", Point(137, 262), FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);

	// Display images
	imshow("Image", img);

	waitKey(0);
}