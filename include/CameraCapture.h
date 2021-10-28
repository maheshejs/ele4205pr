#include <opencv2/opencv.hpp>

class CameraCapture : public cv::VideoCapture
{
private:
    /* data */
public:
    //constructeur
    CameraCapture();

    void setResolution();
};