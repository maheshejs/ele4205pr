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

int getValueFromFile(const char *filePath)
{
    int value = 0;
    ifstream file(filePath);
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
    SERVER_MESSAGE serverMsg = {.rawData = 0};

    bool isTooDark = getValueFromFile(ADC_FILE_PATH) > INTENSITY_THRESHOLD;
    bool isButtonPushed = getValueFromFile(BTN_FILE_PATH) == 0;

    serverMsg.f.IDOWN = isTooDark;
    serverMsg.f.PUSHB = !isTooDark && isButtonPushed;
    serverMsg.f.READY = !isTooDark && !isButtonPushed;

    return serverMsg;
}

int main(int argc, char *argv[])
{
    tcpSockets sockets = initServer();

    CameraCapture capture;

    CLIENT_MESSAGE clientMsg = {.rawData = 0};
    uint32_t CURRENT_RES = 0;
    int n = 0;
    Mat frame;

    while (!clientMsg.f.QUIT)
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

            n = recv(sockets.comm, &clientMsg, sizeof(CLIENT_MESSAGE), MSG_WAITALL);

            if (clientMsg.f.RES != CURRENT_RES)
                capture.setResolution(CURRENT_RES = clientMsg.f.RES);
        }
    }

    close(sockets.comm);
    close(sockets.init);

    return 0;
}
