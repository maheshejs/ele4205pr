
#include "CameraCapture.h"

CameraCapture::CameraCapture() : VideoCapture(0)
{
    if (!(this->isOpened()))
        throw std::runtime_error("Failed to connect to the camera.");
}

void CameraCapture::setResolution()
{
    //To be changed
    this->set(cv::CAP_PROP_FRAME_WIDTH, 800);
    this->set(cv::CAP_PROP_FRAME_HEIGHT, 600);
}
