#include<iostream>
#include<opencv2/opencv.hpp>

#define NUMBER_OF_RESOLUTIONS 13

using namespace std;
using namespace cv;

struct timing {
	int resX;
	int resY;
	double fps;
};

struct timing usbCameraTimings[NUMBER_OF_RESOLUTIONS] = {
        {176, 144, 30.0}, {160, 120, 30.0}, {320, 176, 30.0}, {320, 240, 30.0},
        {352, 288, 30.0}, {432, 240, 30.0}, {800, 600, 30.0}, {864, 480, 30.0},
        {960, 544, 30.0}, {960, 720, 30.0}, {1184, 656, 30.0}, {1280, 720, 30.0},
		{1280, 960, 30.0}
};

int main()
{
	int resolutionChoice = 0;
	cout << "List of supported resolutions : " << endl;
	for (int i = 0; i < NUMBER_OF_RESOLUTIONS; i++)
	{
		cout << i+1 << ") " << usbCameraTimings[i].resX << "x" << usbCameraTimings[i].resY << endl;
	}
	cout << "Please select resolution : ";
	cin >> resolutionChoice;

	double fps = usbCameraTimings[resolutionChoice - 1].fps;
	int resX = usbCameraTimings[resolutionChoice - 1].resX;
	int resY = usbCameraTimings[resolutionChoice - 1].resY;
	double length = 5.0;
	VideoWriter out("capture-liv1.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, Size(resX, resY));
	VideoCapture capture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, resX);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, resY);
	if(!capture.isOpened()){
		cout << "Failed to connect to the camera." << endl;
	}

	Mat frame;
	int frameCount = static_cast<int>(fps*length);

	for(int i = 0; i < frameCount; i++){
		capture >> frame;
		if (frame.empty())
		{
			cout << "Failed to capture an image" << endl;
			return -1;
		}
		else
		{
			out.write(frame);
		}
	}
	return 0;
}
