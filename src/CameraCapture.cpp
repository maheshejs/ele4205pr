#include "CameraCapture.h"

CameraCapture::CameraCapture() : VideoCapture(0)
{
    if (!(this->isOpened()))
        throw std::runtime_error("Failed to connect to the camera.");

	this->setResolution(0);
}

void CameraCapture::setResolution(uint32_t res)
{
    //To be changed
    this->set(CV_CAP_PROP_FRAME_WIDTH, USB_CAMERA_RESOLUTIONS[res].x);
    this->set(CV_CAP_PROP_FRAME_HEIGHT, USB_CAMERA_RESOLUTIONS[res].y);
}
