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

/**
 * \brief 
 * 
 */
class ImageServer: public TCPServer{
public:
    /**
     * \brief Construct a new Image Server object
     * 
     * \param port 
     */
    ImageServer(int port);
    /**
     * \brief Destroy the Image Server object
     * 
     */
    virtual ~ImageServer();

    /**
     * \brief 
     * 
     * \return IMAGE_SERVER_MSG 
     */
    IMAGE_SERVER_MSG computeMessage();
    /**
     * \brief 
     * 
     * \param filePath 
     * \return int 
     */
    static int readFile(std::string filePath); 
private:
    IMAGE_SERVER_MSG _serverMsg;
};

#endif /* IMAGESERVER_H_ */
