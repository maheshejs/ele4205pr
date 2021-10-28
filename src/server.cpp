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

using namespace std;
using namespace cv;

tcpSockets initServer()
{
	int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addr_len;

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT_NUMBER);

    addr_len = sizeof(serv_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
		throw runtime_error("Could not create socket.");

    int bind_result = bind(sockfd, (const sockaddr *)&serv_addr, addr_len);

    if (bind_result < 0) 
		throw runtime_error("Bind failed");

    if (listen(sockfd, 1) < 0) 
		throw runtime_error("Listen failed");

    cout << "Listening on port 4099" << endl;
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_len);

    if (newsockfd < 0) 
		throw runtime_error("Connection refused");

    cout << "Connection accepted" << endl;
	return {sockfd, newsockfd};
}

int main(int argc, char *argv[])
{
    tcpSockets sockets = initServer();

  	CameraCapture capture;
    
	int n;
    MESSAGE msg = {.rawData = 0};
    Mat frame;

    while (1)
    {
        capture >> frame;

	    if (frame.empty()) 
			throw runtime_error("Failed to capture image");

        // Send the header
        n = write(sockets.comm, &frame, sizeof(Mat));

        // Send the data
        int frame_size = frame.total() * frame.elemSize();

        for (long long int bytes_sent = 0; bytes_sent != frame_size; bytes_sent += n)
        {
            int block_size = frame_size - bytes_sent < BUFFER_SIZE ? frame_size - bytes_sent : BUFFER_SIZE;
            n = write(sockets.comm, frame.data + bytes_sent, block_size);
        }

        n = recv(sockets.comm, &msg, sizeof(msg), MSG_WAITALL);

        if (msg.f.QUIT)
            break;
    }

    close(sockets.comm);
    close(sockets.init);

    return 0;
}
