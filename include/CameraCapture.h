#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <opencv2/opencv.hpp>
#include "common.h"

class CameraCapture : public cv::VideoCapture
{
private:
    /* data */
public:
    //constructeur
    CameraCapture();

    void setResolution(uint32_t res);
};

#endif
