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

    Mat header;
	CLIENT_MESSAGE msg = {.rawData = 0};
	SERVER_MESSAGE serverMsg = {.rawData = 0};
	bool buttonHasBeenPressed = false;
	int n;
	int captureCount = 0;

    while (1)
    {
		// Receive server message
		n = recv(sockfd, &serverMsg, sizeof(SERVER_MESSAGE), MSG_WAITALL);

		if (!serverMsg.f.IDOWN)
		{
		    // Receive the header
		    n = recv(sockfd, &header, sizeof(Mat), MSG_WAITALL);

		    // Receive data
		    int frame_size = header.rows * header.cols * CV_ELEM_SIZE(header.flags);
		    uchar data[frame_size];
		    for (long long int bytes_received = 0; bytes_received != frame_size; bytes_received += n)
		    {
		        int block_size = frame_size - bytes_received < BUFFER_SIZE ? frame_size - bytes_received : BUFFER_SIZE;
		        n = read(sockfd, data + bytes_received, block_size);
		    }
		    Mat frame(header.rows, header.cols, header.type(), data);
			
			if (serverMsg.f.PUSHB && !buttonHasBeenPressed)
			{
				captureCount++;
				// Save image if button is pushed
				if (fork() == 0)
				{
					string fileName = "frame" + to_string(captureCount) + ".png";
					imwrite(fileName.c_str(), frame);
					return 0;
				}
			}

			buttonHasBeenPressed = serverMsg.f.PUSHB;

			// Display video
		    imshow("Frame", frame);

		    int keyFlags = waitKey(30);
			int key = keyFlags & 0xFF;
			msg.f.OK = 0;

			if (key == KEY_ESC)
		    {
		        msg.f.QUIT = 1;
		        n = write(sockfd, &msg, sizeof(CLIENT_MESSAGE));
		        break;
		    }
		    else
		    {
				switch(key)
				{
					case '1':case '2':case '3':case '4':
						msg.f.RES = key - '1';
					default:
						msg.f.OK = 1;
						n = write(sockfd, &msg, sizeof(CLIENT_MESSAGE));
						break;
				}
		    }
		}
    }

    close(sockfd);
    return 0;
}
