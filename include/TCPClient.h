/*
 * 
 *
 *  Created on: 
 *      Author: 
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

class TCPClient{
public:
    TCPClient(int serv_port, std::string serv_addr);
    virtual ~TCPClient();

    void initSocket();
    int getCommSocket() const;
private:
    struct sockaddr_in _serv_addr;
    int _sockfd;
};

#endif /* TCPCLIENT_H_ */
