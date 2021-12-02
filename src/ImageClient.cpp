/**
 * \file ImageClient.cpp
 * \date 2 décembre 2021 
 * \author Simon Larivière et Joseph Maheshe
**/


#include "ImageClient.h"

ImageClient::ImageClient(int serv_port, std::string serv_addr) : TCPClient(serv_port, serv_addr)
{
    _clientMsg.rawData = 0;
}

ImageClient::~ImageClient()
{
}

IMAGE_CLIENT_MSG ImageClient::computeMessage(int key)
{
    switch (key)
    {
        case KEY_ESC:
            _clientMsg.f.OK = 0;
            _clientMsg.f.QUIT = 1;
            break;
        case '1': case '2': case '3': case '4':
            _clientMsg.f.RES = key - '1';
        default:
            _clientMsg.f.OK = 1;
            break;
    }
    return _clientMsg;
}

