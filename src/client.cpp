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

    // Réception du header
    char header[sizeof(Mat)];
    n = read(sockfd, header, sizeof(Mat));
    Mat *pheader = reinterpret_cast<Mat *>(header);

    long long int bytes_received = 0;
    char* buffer = new char[BUFFER_SIZE];

    // Reception des data
    //cout << pheader->rows << endl;
    //cout << pheader->cols << endl;
    //cout << pheader->flags << endl;
    int frame_size = pheader->rows * pheader->cols * CV_ELEM_SIZE(pheader->flags);
    uchar* data = new uchar[frame_size];
    while (bytes_received != frame_size)
    {
        int block_size = frame_size - bytes_received < BUFFER_SIZE ? frame_size - bytes_received : BUFFER_SIZE;
        n = read(sockfd, buffer, block_size);
        memcpy(data + bytes_received, buffer, n);
        bytes_received += n;
    }
    cout << "Data all received" << endl;
    cout << "Bytes received : " << bytes_received << endl;
    Mat frame = Mat(pheader->rows, pheader->cols, pheader->type(), data);

    imshow("Frame", frame);
    waitKey(0);

    close(sockfd);
    delete buffer;
    delete data;

    return 0;
}
