/* boneCVtiming.cpp
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */

#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>

#define NUMBER_OF_RESOLUTIONS 13

using namespace std;
using namespace cv;

struct timing {
	int resX;
	int resY;
	double fps;
};

int main()
{
	struct timing usbCameraTimings[NUMBER_OF_RESOLUTIONS] = {
        {176, 144, 30.0}, {160, 120, 30.0}, {320, 176, 30.0}, {320, 240, 30.0},
        {352, 288, 30.0}, {432, 240, 30.0}, {800, 600, 30.0}, {864, 480, 30.0},
        {960, 544, 30.0}, {960, 720, 30.0}, {1184, 656, 30.0}, {1280, 720, 30.0},
		{1280, 960, 30.0}
    };

    for (int i = 0; i < NUMBER_OF_RESOLUTIONS; i++)
    {
		cout << "Calculating framerate for " << usbCameraTimings[i].resX << "x" << usbCameraTimings[i].resY << endl;
		VideoCapture capture(0);
		capture.set(CV_CAP_PROP_FRAME_WIDTH, usbCameraTimings[i].resX);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, usbCameraTimings[i].resY);
		if(!capture.isOpened()){
			cout << "Failed to connect to the camera." << endl;
		}
		Mat frame, edges;

		// We read 2 frames to make sure new resolution is well initialized before measuring time
		capture >> frame;
		capture >> frame;

		struct timespec start, end;
		clock_gettime( CLOCK_REALTIME, &start );

		int frames = 10;
		for(int j = 0; j < frames; j++){
			capture >> frame;
			if(frame.empty()){
			cout << "Failed to capture an image" << endl;
			return -1;
			}
			cvtColor(frame, edges, CV_BGR2GRAY);
			Canny(edges, edges, 0, 30, 3);
		}

		clock_gettime( CLOCK_REALTIME, &end );
		double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/1000000000.0d;
		usbCameraTimings[i].fps = frames/difference;

		imwrite("edges.png", edges);
		imwrite("capture.png", frame);
    }

	cout << "Framerate for every resolution :" << endl;
	for (int i = 0; i < NUMBER_OF_RESOLUTIONS; i++)
	{
		cout << usbCameraTimings[i].resX << "x" << usbCameraTimings[i].resY << " : " << usbCameraTimings[i].fps << " frames/second" << endl;
	}

    return 0;
}
