/*
 * TCPClient.cpp
 *
 *  
 *      Author: 
 */

#include "TCPClient.h"

TCPClient::TCPClient(int serv_port, std::string serv_addr){
    _serv_addr.sin_family = AF_INET;
    inet_aton(serv_addr.c_str(), &_serv_addr.sin_addr);
    _serv_addr.sin_port = htons(serv_port);
}

TCPClient::~TCPClient() {
    close(_sockfd);
}

void TCPClient::initSocket()
{
    socklen_t addr_len = sizeof(_serv_addr);
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (_sockfd < 0)
        throw std::runtime_error("Could not create socket");

    int connected = connect(_sockfd, (const sockaddr *)&_serv_addr, addr_len);
    if (connected < 0)
        throw std::runtime_error("Connection failed");
}

int TCPClient::getCommSocket() const
{
    return _sockfd;
}