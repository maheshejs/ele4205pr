/*
 * client.cpp
 */

#include "common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include<opencv2/opencv.hpp>

#define PORT_NUMBER 4099

using namespace std;
using namespace cv;

int main( int argc, char *argv[] ) {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    socklen_t addr_len;
    
    if (argc != 2)
    {
            perror("Invalid number of arguments. Usage: client <serveraddress>");
            return 0;
    }

    serv_addr.sin_family = AF_INET;
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(PORT_NUMBER);    
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Could not create socket");
        return 0;
    }
    
    addr_len = sizeof(serv_addr);
    
    int connected = connect(sockfd, (const sockaddr *)&serv_addr, addr_len);

    if (connected < 0) {
        perror("Connection failed");
        return 0;
    }

    // Réception du header
    char header[sizeof(Mat)];

    n = read(sockfd, header, sizeof(Mat));
    Mat* recv_mat = reinterpret_cast<Mat*>(header);

    cout << "Flags : " << recv_mat->flags << endl;
    cout << "Dims  : " << recv_mat->dims << endl;
    cout << "Rows : " << recv_mat->rows << endl;
    cout << "Cols : " << recv_mat->cols << endl;

    long long int bytes_received = 0;
    char buffer[BUFFER_SIZE];

    n = read(sockfd, buffer, BUFFER_SIZE);

    while (bytes_received != ipkt.filesize)
    {
        // On lit le stream TCP: 
        
        //Vous devriez distinguer deux cas:
        if( BUFFER_SIZE > ipkt.filesize - bytes_received) 
        {
        //Consigne 14: Cas 1: le nombre de bytes restants est inférieur à BUFFER_SIZE 
        //Vous pouvez utiliser la fonction read() sur le socket du serveur. read() sur un socket est equivalent à la reception des donnees
            n = read(sockfd, &buffer, ipkt.filesize - bytes_received);
        }
        else 
        {
        //Consigne 15: Cas 2: le nombre de bytes restants est supérieur ou égal à BUFFER_SIZE 
            n = read(sockfd, &buffer, sizeof(buffer));
        }

        // Consigne 16: Écrivez les bytes reçus dans le fichier binare ofstream en utilisant ofs.write()
        
        ofs.write(buffer, n);
        
        // Le compteur du nombre de bytes reçu est incrémenté par le nombre de bytes reçus. 
        //Pensez à mettre à jour la valeur de la variable n dans chacun des cas étudié ci-haut
        bytes_received += n;
    }
    
    
    //Etape 2: Envoi du paquet d'initialisation du transfert des donness (il s'agit d'un fichier dans notre cas)
    //Consigne 12: Utilisez la fonction write() pour en envoyer le paquet au serveur 
    //Remarque: write() sur le socket TCP est equivalent à la fonction send() et read() sur un socket TCP est equivalent a receive())
    
    
    written = write(sockfd, &ipkt, sizeof(ipkt));
                                                         
    // Consigne 13: Verifier la valeur de la variable retournee par write() 
    // Affichez un message d'erreur dans le cas oÃ¹ on n'est pas capable d'Ã©crire dans le socket (ce cas correspond Ã  une valeur nÃ©gative retournÃ©e par write())

    if (written < 0)
    {
        perror("Unable to write in socket");
        return 0;
    }

    //==================================Reception de l'acceptation du transfert du fichier de la part du serveur                                              
    // Consigne 14: Reception de l'acceptation du transfert du fichier de la part du serveur. Pensez à utiliser la fonction recv() et le flag  MSG_WAITALL
    MESSAGE msg;
    recv(sockfd, &msg, sizeof(msg), MSG_WAITALL);
    
    // Consigne 15: Affichez un message d'erreur dans le cas msg  n'est pas MSG_ACCEPT puis fermez immediatement le socket ouvert 
    
    if (msg != MSG_ACCEPT)
    {
        perror("The package received was not an accept package.");
        close(sockfd);
        return 0;
    }
                                       
    //===================================Transfert du fichier===============================================

    // la variable bytes_sent stocke le nombre de bytes deja envoyes
    long long int bytes_sent = 0;
    // la variable buffer est un tampon pour stocker les morceaux lus à partir du fichier original qu'on desire envoyer au serveur. Il a un taille maximale BUFFER_SIZE
    char buffer[BUFFER_SIZE];
    
    // Tant qu'on n'a pas envoye tous les bytes du fichier, on va faire le traitement suivant
    while (bytes_sent != ipkt.filesize)
    {
        // Consigne 16: Lecture du fichier: la lecture se fait par fragment de taille BUFFER_SIZE en utilisant la fonction read() sur le ifstream 
        // et mettez cette quantité de données dans buffer
        
        ifs.read(buffer, BUFFER_SIZE);
        
        // Consigne 17: On ecrit les bytes lus : à partir du fichier ifstream dans le stream TCP (ceci est equivalent a les envoyer)
        // ===Attention: vous devez distinguer deux cas: 1) le nombre de bytes retants < BUFFER_SIZE et 2) le nombre de bytes restants >= BUFFER_SIZE. 
        // Adaptez les contenu de vos variables selon le cas
        // Indication: Le nombre de bytes est plus petit que la taille du tampon si on a atteint la fin du fichier au cours d'une lecture 
        // d'une portion du fichier de taille BUFFER_SIZE, Pensez à utiliser la fonction eof() 
        
        if (ipkt.filesize - bytes_sent < BUFFER_SIZE)
        {
            n = write(sockfd, &buffer, ipkt.filesize - bytes_sent);
        }
        else
        {
            n = write(sockfd, &buffer, BUFFER_SIZE);
        }

        // compteur de bytes envoyes est incrementé par le nombre de bytes envoyés
        bytes_sent += n;
    }
//==============================Fermeture de la connexion entre le client et le serveur=================================
    // Consigne 18: Le client envoie le message MSG_END pour fermer la connexion en utilisant la fonction write() sur le socket
    // Affichez un message d'erreur dans si la valeur retournee par write() est negative
    msg = MSG_END;
    written = write(sockfd, &msg, sizeof(msg));

    // Consigne 19: Fermez le ifstream et le socket en utilisant close
    */
    close(sockfd);

    return 0;
}
