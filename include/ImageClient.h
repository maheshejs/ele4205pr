/*
 * 
 *
 *  Created on: 
 *      Author: 
 */

#ifndef IMAGECLIENT_H_
#define IMAGECLIENT_H_

#include "common.h"
#include <TCPClient.h>
#include <string>

const int KEY_ESC = 27;

/**
 * \ class ImageClient
 * ImageClient est une classe héritant de TCPClient (permettant la gestion de la connexion TCP/IP du côté client) et
 * fournissant en plus une méthode pour envoyer les messages client.
 * **/
class ImageClient: public TCPClient{
public:
    ImageClient(int serv_port, std::string serv_addr);
    virtual ~ImageClient();

    IMAGE_CLIENT_MSG computeMessage(int key);
private:
    IMAGE_CLIENT_MSG _clientMsg;
};

#endif /* IMAGECLIENT_H_ */
