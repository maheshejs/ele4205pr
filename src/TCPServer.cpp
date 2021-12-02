/**
 *  \file TCPServer.cpp
 *
 *  \date 2 décembre 2021
 *  \author Simon Larivière et Joseph Maheshe
 */

#include "TCPServer.h"
#include <stdexcept>

TCPServer::TCPServer(int port) {
    _port = port;
    memset(&_serv_addr, 0, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_addr.sin_port = htons(_port);
}

void TCPServer::initSocket()
{
    socklen_t addr_len = sizeof(_serv_addr);
    struct sockaddr_in cli_addr;

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        throw std::runtime_error("Could not create socket.");

    int bind_result = bind(_sockfd, (const sockaddr *)&_serv_addr, addr_len);

    if (bind_result < 0)
        throw std::runtime_error("Bind failed");

    if (listen(_sockfd, 1) < 0)
        throw std::runtime_error("Listen failed");

    std::cout << "Listening on port " << _port << std::endl;
    _newsockfd = accept(_sockfd, (struct sockaddr *)&cli_addr, &addr_len);

    if (_newsockfd < 0)
        throw std::runtime_error("Connection refused");

    std::cout << "Connection accepted" << std::endl;
}

TCPServer::~TCPServer() {
    close(_newsockfd);
    close(_sockfd);
}

int TCPServer::getCommSocket() const
{
    return _newsockfd;
}
