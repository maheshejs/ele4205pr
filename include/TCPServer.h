/**
 *  \file TCPServer.h
 *
 *  \date 2 décembre 2021
 *  \author Simon Larivière et Joseph Maheshe
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
 * \brief TCPServer est une classe de base qui fournit les méthodes permettant d'initier une connexion TCP/IP côté serveur 
 * 
 */
class TCPServer{
public:
    /**
     * \brief Constructeur permettant d'initialiser le port pour l'objet TCPServer
     * 
     * \param port Le port d'écoute
     */
    TCPServer(int port);
    /**
     * \brief Destructeur de l'objet TCPServer
     * 
     */
    virtual ~TCPServer();

    /**
     * \brief Fonction permettant d'initialiser le socket d'écoute et le socket de communication avec le client
     * 
     */
    void initSocket();
    /**
     * \brief Fonction permettant d'obtenir le descripteur du socket de communication
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
