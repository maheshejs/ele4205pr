/**
 * \file TCPClient.h
 * \date 2 décembre 2021 
 * \author Simon Larivière et Joseph Maheshe
**/

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

/**
 * \class TCPClient
 * \brief TCPClient est une classe de base qui fournit les méthodes permettant d'initier une connexion TCP/IP côté client
**/
class TCPClient{
    public:
        /**
         * \fn TCPClient(int serv_port, std::string serv_addr)
         * \brief Constructeur pour l'objet TCPClient permettant d'initialiser les variables utiles pour la connexion au serveur
         * \param serv_port
         * Le port du serveur sur lequel se connecter
         * \param serv_addr
         * L'adresse IP du serveur sur lequel se connecter
        **/
        TCPClient(int serv_port, std::string serv_addr);
        /**
         * \fn ~TCPClient()
         * brief Destructeur de l'objet TCPClient
        **/
        virtual ~TCPClient();

        /**
         * \fn initSocket()
         * \brief Fonction permettant d'initier le socket de communication et établir la connexion avec le serveur
        **/
        void initSocket();
        /**
         * \fn getCommSocket()
         * \brief Fonction permettant d'obtenir le descripteur du socket de communication
         * \return int Le descripteur de socket de communication
        **/
        int getCommSocket() const;
    private:
        struct sockaddr_in _serv_addr;
        int _sockfd;
};

#endif /* TCPCLIENT_H_ */
