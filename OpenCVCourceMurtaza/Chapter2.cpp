#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // using this
#include <iostream>

using namespace cv;
using namespace std;

// basic functions

void main()
{
	string path = "Resources/test.png";
	Mat img = imread(path);

	// Grey
	Mat imgGrey;
	cvtColor(img, imgGrey, COLOR_BGR2GRAY);

	// Blur
	Mat imgBlur;
	GaussianBlur(imgGrey, imgBlur, Size(3, 3), 3, 0); // kernal=3x3, 

	// Edge detection
	Mat imgCanny;
	Canny(imgBlur, imgCanny, 50, 150);

	// Dialate
	Mat imgDia;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3)); // creates kernal, increase size = dialate more, use odd numbers
	dilate(imgCanny, imgDia, kernel);

	// Erode
	Mat imgErode;
	erode(imgDia, imgErode, kernel); // doesnt have to be the same kernel

	// Display images
	imshow("Image", img);
	imshow("Image Grey", imgGrey);
	imshow("Image Blur", imgBlur);
	imshow("Image Canny", imgCanny);
	imshow("Image Dialate", imgDia);
	imshow("Image Erode", imgErode);

	waitKey(0);
}