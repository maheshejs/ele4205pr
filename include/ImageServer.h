/*
 *  ImageServer.h 
 *
 *  Date    : 2 décembre 2021
 *  Auteurs : Simon Larivière et Joseph Maheshe
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
 * \brief ImageServer est une classe héritant de TCPServer qui fournit en plus une méthode pour envoyer un message au client
 * 
 */
class ImageServer: public TCPServer{
public:
    /**
     * \brief Constructeur permettant d'initialiser le port pour l'objet ImageServer
     * 
     * \param port Le port d'écoute 
     */
    ImageServer(int port);
    /**
     * \brief Destructeur de l'objet ImageServer
     * 
     */
    virtual ~ImageServer();

    /**
     * \brief Fonction permettant de déterminer le message (état du bouton-poussoir, luminosité du milieu) à envoyer au client
     * 
     * \return IMAGE_SERVER_MSG Le message à envoyer au client
     */
    IMAGE_SERVER_MSG computeMessage();
    /**
     * \brief Fonction permettant de lire une valeur d'un fichier
     * 
     * \param filePath Le chemin du fichier à lire
     * \return int La valeur lue
     */
    static int readFile(std::string filePath); 
private:
    IMAGE_SERVER_MSG _serverMsg;
};

#endif /* IMAGESERVER_H_ */
