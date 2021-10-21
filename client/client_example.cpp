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

using namespace std;


int main( int argc, char *argv[] ) {

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	socklen_t addr_len;

	// Pour executer l'application client: l'utilisateur devrait saisir trois arguments: l'adresse IP du serveur, le port sollicité sur le serveur et le nom du fichier a transferer
	// Remarque: le fichier utilisé en argument doit etre dans le même dossier que l'executable
	// Remarque: argv[] est un tableau dans lequel sont stockés les arguments saisis. Le nombre des arguments saisis est stocké dans la variable argc
	
	// Consigne1: Affichez un message d'erreur lorsque le nombre d'arguments est insuffisant, (indication: pensez a utiliser la valeur de argc)
	
    if (argc != 4)
    {
            perror("Invalid number of arguments. Usage: client <serveraddress> <portnumber> <filename>");
            return 0;
    }

	// Consigne 2: Vérifiez la longueur du nom du fichier a envoyer (ceci se trouve dans argv[3]) 
	// et affichez un message d'erreur lorsque la longeur de son nom depasse la taille maximale FILENAME_MAX_SIZE. 
	// Pensez a  utiliser la fonction strlen() pour avoir sa taille 
	
	if (strlen(argv[3]) > FILENAME_MAX_SIZE)
	{
		perror("Name of file is too long.");
		return 0;
	}
						  
	//Consigne 3: Initialisation des attributs de la variable structure serv_addr (sin_family, sin_addr et sin_port)
    //Inspirez-vous du code que vous avez fait en prelab
	
	serv_addr.sin_family = AF_INET;
	inet_aton(argv[1], &serv_addr.sin_addr);
	serv_addr.sin_port = htons(atoi(argv[2]));	

	// Consigne 4: Creez un socket en utilisant la fonction socket()
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//Consigne 5: Verfiez la valeur de la variable retournee par socket() et affichez un message d'erreur lorsque la creation du socket a echoue
	
	if (sockfd < 0)
	{
		perror("Could not create socket");
		return 0;
	}

	//Consigne 6: recupererz la taille de serv_addr et stockez-là dans la variable pour une utilisation ulterieure avec les fonctions send(). etc,
	//(pensez à utiliser sizeof()) 
	
	addr_len = sizeof(serv_addr);
									  
	//=========================Ouverture de la connexion entre le client et le serveur========================
	
	// Consigne 7: connectez le socket client au serveur en utilisant la fonction connect()
	
	int connected = connect(sockfd, (const sockaddr *)&serv_addr, addr_len);

	//Consigne 8: Verifiez la valeur de la variable retournÃ©e par la finction connect() et affichez un message d'erreur lorsque la connexion au serveur Ã©choue
	 
	if (connected < 0) {
		perror("Connection failed");
		return 0;
	}
	 
	// Consigne 9: Ouvrez un ifstream binaire (fichier en lecture binaire) associé au fichier saisi dans les arguments
	//Remarque: le nom fichier se trouve dans argv[3]
	
	ifstream ifs (argv[3], ifstream::in);
	
	//===============================initialisation du transfert du fichier======================================
	//Etape 1: creation du paquet d'initialisation du transfert des donness (il s'agit d'un fichier dans notre cas)
	
	init_packet ipkt;
	memset(&ipkt, 0, sizeof(init_packet));
	// ipkt est une variable de type structure (voir sa definition dans common.h). 
	// ipkt est utilise dans le reste du code pour definir le paquet d'initialisation du transfert des donnees
		
	// Consigne 10: Initialisez l'entete du paquet d'initialisation du transfert des donness ipkt par MSG_INIT 
	
	ipkt.msg = MSG_INIT;
	
	// Consigne 11: Initialisez le champ ipkt.filesize par la taille du fichier en utilisant la fonction getFileSize() definie dans common.h et common.cpp
	
	ipkt.filesize = getFileSize(argv[3]);
										   
	// Initialisation du champ ipkt.filename par le nom du fichier en utilisant la fonction copy()
	string(argv[3]).copy(ipkt.filename, strlen(argv[3]));
	
	//Etape 2: Envoi du paquet d'initialisation du transfert des donness (il s'agit d'un fichier dans notre cas)
	//Consigne 12: Utilisez la fonction write() pour en envoyer le paquet au serveur 
	//Remarque: write() sur le socket TCP est equivalent à la fonction send() et read() sur un socket TCP est equivalent a receive())
	
	int written;
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
	
	ifs.close();
	close(sockfd);

	return 0;
}
