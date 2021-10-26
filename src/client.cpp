/*
 * client.cpp
 */

#include "common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#define PORT_NUMBER 4099

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    socklen_t addr_len;

    if (argc != 2)
    {
        perror("Invalid number of arguments. Usage: client <serveraddress>");
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(PORT_NUMBER);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Could not create socket");
        return 0;
    }

    addr_len = sizeof(serv_addr);

    int connected = connect(sockfd, (const sockaddr *)&serv_addr, addr_len);
    if (connected < 0)
    {
        perror("Connection failed");
        return 0;
    }

    Mat header;
    while (1)
    {
        // Réception du header
        n = recv(sockfd, &header, sizeof(Mat), MSG_WAITALL);

        // Reception des data
        int frame_size = header.rows * header.cols * CV_ELEM_SIZE(header.flags);
        uchar data[frame_size];
        for (long long int bytes_received = 0; bytes_received != frame_size; bytes_received += n)
        {
            int block_size = frame_size - bytes_received < BUFFER_SIZE ? frame_size - bytes_received : BUFFER_SIZE;
            n = read(sockfd, data + bytes_received, block_size);
        }
        Mat frame(header.rows, header.cols, header.type(), data);

        imshow("Frame", frame);
        int key = waitKey(30);
        MESSAGE msg = {.rawData = 0};
        if ((key & 0xFF) == 27)
        {
            msg.f.QUIT = 1;
            n = write(sockfd, &msg, sizeof(msg));
            break;
        }
        else
        {
            msg.f.OK = 1;
            n = write(sockfd, &msg, sizeof(msg));
        }
    }

    close(sockfd);
    return 0;
}
