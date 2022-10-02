#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Document scanner

Mat preProcessing(Mat img);
vector<Point> getContours(Mat imgDia);
void drawPoints(vector<Point>& points, Scalar color);

Mat imgOriginal, imgGrey, imgCanny, imgThre, imgBlur, imgDia;
vector<Point> initialPoints;

void main()
{
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);

	// scale image
	resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	// Preprocessing
	imgThre = preProcessing(imgOriginal);

	// Get contours (assume biggest rectangle is the paper)
	initialPoints = getContours(imgThre);
	drawPoints(initialPoints, Scalar(0, 0, 255));
	// Warp


	imshow("Image", imgOriginal);
	imshow("Image Dialation", imgThre);
	waitKey(0);
}

void drawPoints(vector<Point>& points, Scalar color)
{
	int i{};
	for (auto& p : points)
	{
		circle(imgOriginal, p, 10, color, FILLED);
		putText(imgOriginal, to_string(i++), p, FONT_HERSHEY_PLAIN, 4, color, 4);
	}
}

Mat preProcessing(Mat img)
{
	// Grey
	cvtColor(img, imgGrey, COLOR_BGR2GRAY);

	// Blur
	GaussianBlur(imgGrey, imgBlur, Size(3, 3), 3, 0); // kernal=3x3, 

	// Edge detection
	Canny(imgBlur, imgCanny, 50, 150);

	// Dialate
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3)); // creates kernal, increase size = dialate more, use odd numbers
	dilate(imgCanny, imgDia, kernel);

	//// Erode
	//Mat imgErode;
	//erode(imgDia, imgErode, kernel); // doesnt have to be the same kernel

	return imgDia;
}

vector<Point> getContours(Mat imgDia)
{
	vector<vector<Point>> contours; // { {Point(20, 30), Point(50, 60) }, {}, {} };
	vector<Vec4i> hierarchy;

	findContours(imgDia, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<Rect> boundRect(contours.size());
	vector<vector<Point>> conPoly(contours.size()); // only corner points

	//Point myPoint(0, 0);
	vector<Point> biggest;
	int maxArea{};

	for (size_t i{}; i < contours.size(); ++i)
	{
		auto area = contourArea(contours.at(i));
		//cout << area << endl;
		string objectType{};

		if (area > 1000)
		{
			auto peri = arcLength(contours.at(i), true);
			approxPolyDP(contours[i], conPoly.at(i), 0.02 * peri, true);

			if (area > maxArea && conPoly.at(i).size() == 4) // also check if it is a rect
			{
				drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 3);
				biggest = { conPoly.at(i).at(0), conPoly.at(i).at(1), conPoly.at(i).at(2), conPoly.at(i).at(3) };
				maxArea = area;
			}

			//cout << conPoly.at(i).size() << endl;
			//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);
			//rectangle(imgOriginal, boundRect.at(i).tl(), boundRect.at(i).br(), Scalar(0, 255, 0), 5); // tl = top left, br = bottom right

		}
	}

	return biggest;
}
