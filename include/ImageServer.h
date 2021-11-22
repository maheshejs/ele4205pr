/*
 * 
 *
 *  Created on: 
 *      Author: 
 */

#ifndef IMAGESERVER_H_
#define IMAGESERVER_H_

#include "common.h"
#include "TCPServer.h"
#include <fstream>
#include <string>

const std::string ADC_PATH = "/sys/class/saradc/ch0";
const std::string BTN_PATH = "/sys/class/gpio/gpio228/value";
const int INTENSITY_THRESHOLD = 128;

class ImageServer: public TCPServer{
public:
    ImageServer(int port);
    virtual ~ImageServer();

    IMAGE_SERVER_MSG computeMessage();
    static int readFile(std::string filePath); 
private:
    IMAGE_SERVER_MSG _serverMsg;
};

#endif /* IMAGESERVER_H_ */
