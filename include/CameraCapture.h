#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <opencv2/opencv.hpp>

#define NUMBER_OF_RESOLUTIONS 13

struct resolution
{
    int x;
    int y;
};

const resolution USB_CAMERA_RESOLUTIONS[NUMBER_OF_RESOLUTIONS] = {
    {160, 120},
    {320, 240},
    {800, 600},
    {1280, 720},
    {176, 144},
    {320, 176},
    {352, 288},
    {432, 240},
    {864, 480},
    {960, 544},
    {960, 720},
    {1184, 656},
    {1280, 960}
};

/**
 * \brief 
 * 
 */
class CameraCapture : public cv::VideoCapture
{
public:
    /**
     * \brief Construct a new Camera Capture object
     * 
     */
    CameraCapture();

    /**
     * \brief Set the Resolution object
     * 
     * \param ID_RESOLUTION 
     */
    void setResolution(uint32_t ID_RESOLUTION);
};

#endif
