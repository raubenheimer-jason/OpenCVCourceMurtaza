#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Shapes and contour detection

void getContours(Mat imgDia, Mat img);

void main()
{
	string path = "Resources/shapes.png";
	Mat img = imread(path);

	Mat imgGrey, imgBlur, imgCanny, imgDia;

	cvtColor(img, imgGrey, COLOR_BGR2GRAY);
	GaussianBlur(imgGrey, imgBlur, Size(3, 3), 3, 0); // kernal=3x3, 
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3)); // creates kernal, increase size = dialate more, use odd numbers
	dilate(imgCanny, imgDia, kernel);

	getContours(imgDia, img);

	imshow("Image", img);
	//imshow("Image Grey", imgGrey);
	//imshow("Image Blur", imgBlur);
	//imshow("Image Canny", imgCanny);
	//imshow("Image Dia", imgDia);

	waitKey(0);
}

void getContours(Mat imgDia, Mat img)
{
	vector<vector<Point>> contours; // { {Point(20, 30), Point(50, 60) }, {}, {} };
	vector<Vec4i> hierarchy;

	findContours(imgDia, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<Rect> boundRect(contours.size());
	vector<vector<Point>> conPoly(contours.size()); // only corner points

	for (size_t i{}; i < contours.size(); ++i)
	{
		auto area = contourArea(contours.at(i));
		cout << area << endl;
		string objectType{};

		if (area > 1000)
		{
			auto peri = arcLength(contours.at(i), true);
			approxPolyDP(contours[i], conPoly.at(i), 0.02 * peri, true);
			cout << conPoly.at(i).size() << endl;
			boundRect.at(i) = boundingRect(conPoly.at(i));
			auto objCor{ conPoly.at(i).size() }; // object corners

			if (objCor == 3)
				objectType = "Tri";
			else if (objCor == 4)
			{
				double aspRatio{ static_cast<double>(boundRect.at(i).width) / boundRect.at(i).height };
				if (aspRatio > 0.95 && aspRatio < 1.05)
					objectType = "Square";
				else
					objectType = "Rect";
			}
			else if (objCor > 4)
				objectType = "Circle";

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect.at(i).tl(), boundRect.at(i).br(), Scalar(0, 255, 0), 5); // tl = top left, br = bottom right
			putText(img, objectType, { boundRect.at(i).x, boundRect.at(i).y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 2);

		}
	}
}