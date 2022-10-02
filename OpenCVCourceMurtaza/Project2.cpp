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
vector<Point> reorder(vector<Point> initialPoints);
Mat getWarp(Mat imgOriginal, vector<Point> docPoints, float w, float h);

Mat imgOriginal, imgGrey, imgCanny, imgThre, imgBlur, imgDia, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

float w{ 420 }, h{ 596 };

void main()
{
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);

	// scale image
	//resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5); // only for testing

	// Preprocessing
	imgThre = preProcessing(imgOriginal);

	// Get contours (assume biggest rectangle is the paper)
	initialPoints = getContours(imgThre);
	docPoints = reorder(initialPoints);
	//drawPoints(docPoints, Scalar(0, 0, 255));

	// Warp
	imgWarp = getWarp(imgOriginal, docPoints, w, h); // w and h of the A4 paper

	// Crop
	int cropVal{ 5 };
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal)); // 3:42 in video
	imgCrop = imgWarp(roi);

	imshow("Image", imgOriginal);
	//imshow("Image Dialation", imgThre);
	//imshow("Image Warp", imgWarp);
	imshow("Image Crop", imgCrop);

	waitKey(0);
}

Mat getWarp(Mat imgOriginal, vector<Point> docPoints, float w, float h)
{
	Point2f src[4] = { docPoints.at(0), docPoints.at(1), docPoints.at(2), docPoints.at(3) };
	Point2f dst[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f,h}, {w,h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(imgOriginal, imgWarp, matrix, Point(w, h));

	return imgWarp;
}


vector<Point> reorder(vector<Point> initialPoints)
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (auto& p : initialPoints)
	{
		sumPoints.push_back(p.x + p.y);
		subPoints.push_back(p.x - p.y);
	}

	newPoints.push_back(initialPoints.at(min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin())); // index 0
	newPoints.push_back(initialPoints.at(max_element(subPoints.begin(), subPoints.end()) - subPoints.begin())); // index 1
	newPoints.push_back(initialPoints.at(min_element(subPoints.begin(), subPoints.end()) - subPoints.begin())); // index 2
	newPoints.push_back(initialPoints.at(max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin())); // index 3

	return newPoints;
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
	double maxArea{};

	for (int i{}; i < contours.size(); ++i)
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
				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 3);
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
