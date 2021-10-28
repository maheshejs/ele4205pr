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

const char ADC_FILE_PATH[] = "/sys/class/saradc/ch0";
const char BTN_FILE_PATH[] = "/sys/class/gpio/gpio228/value";

const int INTENSITY_THRESHOLD = 128;

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

int getValueFromFile(const char* filePath)
{
	int value;
	ifstream file (filePath);
  	if (file.is_open())
	{
		file >> value;
    	file.close();
		return value;
	}

	throw runtime_error("Unable to open file"); 
}

SERVER_MESSAGE computeServerMessage()
{
	SERVER_MESSAGE serverMessage = {.rawData = 0};

	bool isTooDark = getValueFromFile(ADC_FILE_PATH) > INTENSITY_THRESHOLD;
	bool isButtonPushed = getValueFromFile(BTN_FILE_PATH) == 0;

	serverMessage.f.IDOWN = isTooDark;
	serverMessage.f.PUSHB = !isTooDark && isButtonPushed;
	serverMessage.f.READY = !isTooDark && !isButtonPushed;

	return serverMessage;
}

int main(int argc, char* argv[])
{
    tcpSockets sockets = initServer();

  	CameraCapture capture;
    
	int n;
    CLIENT_MESSAGE msg = {.rawData = 0};
	uint32_t currentRes = 0;
    Mat frame;

    while (1)
    {
        capture >> frame;

	    if (frame.empty()) 
			throw runtime_error("Failed to capture image");

		// Send the server message
		SERVER_MESSAGE serverMsg = computeServerMessage();
		n = write(sockets.comm, &serverMsg, sizeof(SERVER_MESSAGE));

		if (!serverMsg.f.IDOWN)
		{
		    // Send the header
		    n = write(sockets.comm, &frame, sizeof(Mat));

		    // Send the data
		    int frame_size = frame.total() * frame.elemSize();

		    for (long long int bytes_sent = 0; bytes_sent != frame_size; bytes_sent += n)
		    {
		        int block_size = frame_size - bytes_sent < BUFFER_SIZE ? frame_size - bytes_sent : BUFFER_SIZE;
		        n = write(sockets.comm, frame.data + bytes_sent, block_size);
		    }

		    n = recv(sockets.comm, &msg, sizeof(CLIENT_MESSAGE), MSG_WAITALL);

		    if (msg.f.QUIT)
			{
		        break;
			}
			else
			{
				if (msg.f.RES != currentRes)
				{
					currentRes = msg.f.RES;
					capture.setResolution(currentRes);
				}		
			}
		}
    }

    close(sockets.comm);
    close(sockets.init);

    return 0;
}
