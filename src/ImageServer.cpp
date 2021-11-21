/*
 * ImageServer.cpp
 *
 *  
 *      Author: 
 */

#include "ImageServer.h"

ImageServer::ImageServer(int port) : TCPServer(port)
{
    _serverMsg.rawData = 0;
}

ImageServer::~ImageServer() {
}

void ImageServer::computeMessage()
{
    bool isTooDark = readFile(ADC_PATH) > INTENSITY_THRESHOLD;
    bool isButtonPushed = readFile(BTN_PATH) == 0;
    _serverMsg.f.IDOWN = isTooDark;
    _serverMsg.f.PUSHB = !isTooDark && isButtonPushed;
    _serverMsg.f.READY = !isTooDark && !isButtonPushed;
}

IMAGE_SERVER_MSG ImageServer::getMessage() const
{
    return _serverMsg;
}

int ImageServer::readFile(std::string filePath)
{
    int value = 0;
    std::ifstream file(filePath.c_str());
    if (file.is_open())
    {
        file >> value;
        file.close();
        return value;
    }
    throw std::runtime_error("Unable to open file");
}
