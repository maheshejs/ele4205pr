/**
 * \file ImageClient.h
 * \date 2 décembre 2021 
 * \author Simon Larivière et Joseph Maheshe
**/

#ifndef IMAGECLIENT_H_
#define IMAGECLIENT_H_

#include "common.h"
#include <TCPClient.h>
#include <string>

const int KEY_ESC = 27;

/**
 * \class ImageClient
 * \brief ImageClient est une classe héritant de TCPClient qui fournit en plus une méthode pour envoyer les messages au serveur
 * **/
class ImageClient: public TCPClient{
public:
    /**
     * \fn ImageClient(int serv_port, std::string serv_addr)
     * \brief Constructeur permettant d'initialiser les variables utiles pour la connexion au serveur
     * \param serv_port
     * Le port du serveur sur lequel se connecter
     * \param serv_addr
     * L'adresse IP du serveur sur lequel se connecter
    **/
    ImageClient(int serv_port, std::string serv_addr);
    /**
     * \fn ~TCPClient()
     * brief Destructeur de l'objet TCPClient
    **/
    virtual ~ImageClient();

    /**
     * \fn computeMessage(int key)
     * \brief Fonction permettant de déterminer le message à envoyer au serveur
     * \param key
     * La touche du clavier ayant été appuyée, s'il y en a une
     * \return IMAGE_CLIENT_MSG Le message à envoyer au serveur
    **/
    IMAGE_CLIENT_MSG computeMessage(int key);
private:
    IMAGE_CLIENT_MSG _clientMsg;
};

#endif /* IMAGECLIENT_H_ */
