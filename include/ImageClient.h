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

class ImageClient: public TCPClient{
public:
    ImageClient(int serv_port, std::string serv_addr);
    virtual ~ImageClient();

    void computeMessage(int key);
    IMAGE_CLIENT_MSG getMessage() const;
private:
    IMAGE_CLIENT_MSG _clientMsg;
};

#endif /* IMAGECLIENT_H_ */
