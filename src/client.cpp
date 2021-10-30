/*
 * client.cpp
 */

#include "common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstring>

#define KEY_ESC 27

using namespace std;
using namespace cv;

int initClient(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;
    socklen_t addr_len;

    if (argc != 2)
        throw runtime_error("Invalid number of arguments. Usage: client <serveraddress>");

    serv_addr.sin_family = AF_INET;
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(PORT_NUMBER);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        throw runtime_error("Could not create socket");

    addr_len = sizeof(serv_addr);

    int connected = connect(sockfd, (const sockaddr *)&serv_addr, addr_len);
    if (connected < 0)
        throw runtime_error("Connection failed");

    return sockfd;
}

int main(int argc, char *argv[])
{
    int sockfd = initClient(argc, argv);

    CLIENT_MESSAGE clientMsg = {.rawData = 0};
    SERVER_MESSAGE serverMsg = {.rawData = 0};
    uint32_t LAST_PUSHB = 0;
    int n = 0;
    int captureCount = 0;
    Mat header;

    while (!clientMsg.f.QUIT)
    {
        // Receive server message
        n = recv(sockfd, &serverMsg, sizeof(SERVER_MESSAGE), MSG_WAITALL);

        if (!serverMsg.f.IDOWN)
        {
            // Receive the header
            n = recv(sockfd, &header, sizeof(Mat), MSG_WAITALL);

            // Receive data
            int frameSize = header.rows * header.cols * CV_ELEM_SIZE(header.flags);
            uchar data[frameSize];
            for (long long int bytesReceived = 0; bytesReceived != frameSize; bytesReceived += n)
            {
                int blockSize = frameSize - bytesReceived < BUFFER_SIZE ? frameSize - bytesReceived : BUFFER_SIZE;
                n = read(sockfd, data + bytesReceived, blockSize);
            }
            Mat frame(header.rows, header.cols, header.type(), data);

            if (serverMsg.f.PUSHB && !LAST_PUSHB)
            {
                ++captureCount;
                // Save image if button is pushed
                if (fork() == 0)
                {
                    string fileName = "Frame" + to_string(captureCount) + ".png";
                    imwrite(fileName.c_str(), frame);
                    return 0;
                }
            }

            LAST_PUSHB = serverMsg.f.PUSHB;

            // Display video
            imshow("Frame", frame);

            int keyFlags = waitKey(30);
            int key = keyFlags & 0xFF;

            switch (key)
            {
            case KEY_ESC:
                clientMsg.f.OK = 0;
                clientMsg.f.QUIT = 1;
                break;
            case '1': case '2': case '3': case '4':
                clientMsg.f.RES = key - '1';
            default:
                clientMsg.f.OK = 1;
                break;
            }
            n = write(sockfd, &clientMsg, sizeof(CLIENT_MESSAGE));
        }
    }

    close(sockfd);
    return 0;
}
