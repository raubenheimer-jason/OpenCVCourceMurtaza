#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Warp images

void main()
{
	string path = "Resources/cards.jpg";
	Mat img = imread(path);

	// go to Paint to get the pixel positions of the corners of the cards
	Point2f src[4] = { {529,142}, {771,190}, {405,395}, {674,457} };

	// size of cards 2.5 inch x 3.5 inch
	float w = 250.0, h = 350.0;
	Point2f dst[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f,h}, {w,h} };

	Mat matrix, imgWarp;
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	// create circles around the points
	for (size_t i{}; i < 4; ++i)
		circle(img, src[i], 10, Scalar(255, 0, 255), FILLED);

	imshow("Image", img);
	imshow("Image Warp", imgWarp);

	waitKey(0);
}