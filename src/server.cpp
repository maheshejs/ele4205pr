/*
 * server.cpp
 */

#include "ImageServer.h"
#include "CameraCapture.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    ImageServer imgServer(IMAGE_SERVER_PORT);
    imgServer.initSocket();
    CameraCapture capture;

    IMAGE_CLIENT_MSG clientMsg = {.rawData = 0};
    IMAGE_SERVER_MSG serverMsg = {.rawData = 0};
    uint32_t CURRENT_RES = 0;
    int n = 0;
    Mat frame;

    while (!clientMsg.f.QUIT)
    {
        // Send the server message
        imgServer.computeMessage();
        serverMsg = imgServer.getMessage();
        n = write(imgServer.getCommSocket(), &serverMsg, sizeof(IMAGE_SERVER_MSG));

        if (!serverMsg.f.IDOWN)
        {
            capture >> frame;

            if (frame.empty())
                throw runtime_error("Failed to capture image");

            // Send the header
            n = write(imgServer.getCommSocket(), &frame, sizeof(Mat));

            // Send the data
            int frameSize = frame.total() * frame.elemSize();

            for (long long int bytesSent = 0; bytesSent != frameSize; bytesSent += n)
            {
                int blockSize = frameSize - bytesSent < BUFFER_SIZE ? frameSize - bytesSent : BUFFER_SIZE;
                n = write(imgServer.getCommSocket(), frame.data + bytesSent, blockSize);
            }

            n = recv(imgServer.getCommSocket(), &clientMsg, sizeof(IMAGE_CLIENT_MSG), MSG_WAITALL);

            if (clientMsg.f.RES != CURRENT_RES)
                capture.setResolution(CURRENT_RES = clientMsg.f.RES);
        }
    }
    return 0;
}
