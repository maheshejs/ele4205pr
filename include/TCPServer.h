/*
 * 
 *
 *  Created on: 
 *      Author: 
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/**
 * \brief 
 * 
 */
class TCPServer{
public:
    /**
     * \brief Construct a new TCPServer object
     * 
     * \param port 
     */
    TCPServer(int port);
    /**
     * \brief Destroy the TCPServer object
     * 
     */
    virtual ~TCPServer();

    /**
     * \brief 
     * 
     */
    void initSocket();
    /**
     * \brief Get the Comm Socket object
     * 
     * \return int 
     */
    int getCommSocket() const;
private:
    struct sockaddr_in _serv_addr;
    int _port;
    int _sockfd;
    int _newsockfd;
};

#endif /* TCPSERVER_H_ */
