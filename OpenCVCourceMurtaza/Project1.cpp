#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// project 1

/*
Red:
hmin: 0
hmax : 179
smin : 76
smax : 196
vmin : 108
vmax : 166

Blue :
hmin : 97
hmax : 129
smin : 78
smax : 255
vmin : 90
vmax : 233
*/

vector<vector<int>> findColor(Mat img);
Point getContours(Mat imgDia);
void drawOnCanvas(vector<vector<int>> newPoints);

// hmin, smin, vmin, hmax, smax, vmax
vector<vector<int>> myColors{ {0, 76, 108, 179, 196, 166}, {97, 78, 90, 129, 255, 233} }; // red, blue
vector<Scalar> myColorValues{ {0, 0, 255}, {255, 0, 0} }; // red, blue

Mat img;

void main()
{
	VideoCapture cap(0); // imput camera id

	vector<vector<int>> newPoints; // {{x, y, colour}, {}} -> colour = 1 or 2

	while (true)
	{
		cap.read(img);
		newPoints = findColor(img);
		drawOnCanvas(newPoints);
		imshow("Image", img);
		waitKey(1); // 1ms
	}
}

vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	vector<vector<int>> newPoints; // {{x, y, colour}, {}} -> colour = 1 or 2

	int i{};
	for (auto& c : myColors)
	{
		// define limits
		Scalar lower(c.at(0), c.at(1), c.at(2)); // hue, saturation, value
		Scalar upper(c.at(3), c.at(4), c.at(5));
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoint = getContours(mask);

		if (myPoint.x != 0 && myPoint.y != 0)
			newPoints.push_back({ myPoint.x, myPoint.y, i });

		i++;
	}

	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints)
{
	for (auto& np : newPoints)
	{
		circle(img, Point(np.at(0), np.at(1)), 10, myColorValues.at(np.at(2)), FILLED);
	}
}


Point getContours(Mat imgDia)
{
	vector<vector<Point>> contours; // { {Point(20, 30), Point(50, 60) }, {}, {} };
	vector<Vec4i> hierarchy;

	findContours(imgDia, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<Rect> boundRect(contours.size());
	vector<vector<Point>> conPoly(contours.size()); // only corner points

	Point myPoint(0, 0);

	for (size_t i{}; i < contours.size(); ++i)
	{
		auto area = contourArea(contours.at(i));
		//cout << area << endl;
		string objectType{};

		if (area > 1000)
		{
			auto peri = arcLength(contours.at(i), true);
			approxPolyDP(contours[i], conPoly.at(i), 0.02 * peri, true);
			//cout << conPoly.at(i).size() << endl;
			boundRect.at(i) = boundingRect(conPoly.at(i));

			myPoint.x = boundRect.at(i).x + boundRect.at(i).width / 2;
			myPoint.y = boundRect.at(i).y;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect.at(i).tl(), boundRect.at(i).br(), Scalar(0, 255, 0), 5); // tl = top left, br = bottom right

		}
	}

	return myPoint;
}
