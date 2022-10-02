#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Number plate detection


void main()
{
	VideoCapture cap(0); // imput camera id
	Mat img;

	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty())
		cout << ".xml file not loaded" << endl;

	vector<Rect> plates;


	while (true)
	{
		cap.read(img);

		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		int i{};
		for (auto f : plates)
		{
			Mat imgCrop = img(f);
			//imshow(to_string(i++), imgCrop);
			imwrite("Resources/plates/" + to_string(i++) + ".png", imgCrop);
			rectangle(img, f.tl(), f.br(), Scalar(255, 0, 255), 3);
		}

		imshow("Image", img);
		waitKey(1);
	}
}
