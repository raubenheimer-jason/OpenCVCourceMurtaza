#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Resize and crop

void main()
{
	string path = "Resources/test.png";
	Mat img = imread(path);

	// current image size
	cout << img.size() << endl; // [768 x 559]

	// Resize
	Mat imgResize;
	//resize(img, imgResize, Size(640, 480));
	resize(img, imgResize, Size(), 0.5, 0.5); // this scales instead of exact pixels

	// Crop
	Mat imgCrop;
	Rect roi(100, 100, 300, 250); // region of intrest (x, y, width, height), top left x,y
	imgCrop = img(roi);

	// Display images
	imshow("Image", img);
	imshow("Image Resize", imgResize);
	imshow("Image Crop", imgCrop);

	waitKey(0);
}