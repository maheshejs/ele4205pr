/*
 * server.cpp
 */

#include "common.h"
#include "CameraCapture.h"
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <opencv2/opencv.hpp>

#define PORT_NUMBER 4099

using namespace std;
using namespace cv;

/*void captureFrame(Mat &frame)
{
    int resolutionChoice = 6;
    int resX = usbCameraTimings[resolutionChoice].resX;
    int resY = usbCameraTimings[resolutionChoice].resY;
    VideoCapture capture("capture.avi");
    capture.set(CV_CAP_PROP_FRAME_WIDTH, resX);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, resY);
    if (!capture.isOpened())
    {
        cout << "Failed to connect to the camera." << endl;
        return;
    }
    capture >> frame;
    if (frame.empty())
    {
        cout << "Failed to capture an image" << endl;
        return;
    }
    capture.release();
    cout << "Capture complete" << endl;
    return;
}*/

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addr_len;
    int n;

    portno = PORT_NUMBER;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    addr_len = sizeof(serv_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Could not create socket");
        return 0;
    }

    int bind_result = bind(sockfd, (const sockaddr *)&serv_addr, addr_len);

    if (bind_result < 0)
    {
        perror("Bind failed");
        return 0;
    }

    if (listen(sockfd, 1) < 0)
    {
        perror("Listen failed");
        return 0;
    }

    cout << "Listening on port 4099" << endl;
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_len);

    if (newsockfd < 0)
    {
        perror("Connection refused");
        return 0;
    }
    cout << "Connection accepted" << endl;
    // Démarrer la caméra, capturer une image et envoyer l'image au client

    MESSAGE msg = {.rawData = 0};
    Mat frame;

    CameraCapture capture;
    if (!capture.isOpened())
    {
        cout << "Failed to parse video" << endl;
        return 0;
    }
    while (1)
    {
        capture >> frame;
        if (frame.empty())
        {
            cout << "Failed to capture image" << endl;
            return 0;
        }
        // send the header
        n = write(newsockfd, &frame, sizeof(Mat));

        // send the data
        int frame_size = frame.total() * frame.elemSize();
        for (long long int bytes_sent = 0; bytes_sent != frame_size; bytes_sent += n)
        {
            int block_size = frame_size - bytes_sent < BUFFER_SIZE ? frame_size - bytes_sent : BUFFER_SIZE;
            n = write(newsockfd, frame.data + bytes_sent, block_size);
        }
        n = recv(newsockfd, &msg, sizeof(msg), MSG_WAITALL);
        if (msg.f.QUIT)
            break;
    }
    close(newsockfd);
    close(sockfd);

    return 0;
}
