/*
 * main.cpp
 */

#include "common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

int main( int argc, char *argv[] ) {

	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t addr_len;
	int n;
    // Pour executer l'application serveur, l'utilisateur devrait saisir un seul argument qui est le numéro de port sollicité sur le serveur
	// Remarque: le fichier reçu de la part du client se trouvera dans le même dossier que l'executable Server
	// Remarque: argv[] est un tableau dans lequel sont stockés les arguments saisis. Le nombre des arguments saisis est stocké dans la variable argc
	// Consigne 1: Afin de guider l'utilisateur quant au nombre d'arguments requis, affichez un message d'erreur si le nombre d'arguments saisis est incorrect
	
    if (argc != 2)
    {
            perror("Invalid number of arguments. Usage: server <portnumber>");
            return 0;
    }
	
	// Consigne 2: Convertissez la chaine de caractere argv[1] (le numero de port sollicité sur le serveur) en un entier, pensez à utiliser la fonction atoi()

	portno = atoi(argv[1]);
										 
	// Consigne 3: Initialisation des différents attributs sin_family, sin_addr.s_addr et sin_port de la variable serv_addr de type structure. Inspirez-vous de votre code de prelab
	memset(&serv_addr, 0, sizeof(serv_addr));
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;									 
    serv_addr.sin_port = htons(portno);

    addr_len = sizeof(serv_addr);
	// Consigne 4: Creez le socket en utilisant la fonction socket()
	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
								  
    // Consigne 5: Verifiez la variable retournee par la fonction socket() et affichez le message d'erreur correspondant, 
	//Indication: vous pouvez utiliser la fonction perror() 

    if (sockfd < 0)
    {
            perror("Could not create socket");
            return 0;
    }	

	// Consigne 6: On lie socket creer à la structure definie au début du code. Pensez a utiliser la fonction bind()
	
	int bind_result = bind (sockfd, (const sockaddr *)&serv_addr, addr_len);
	
	//Consigne 7: Affichez un message d'erreur lorsque le bind ne reussit pas
	
	if (bind_result < 0)
	{
		perror("Bind failed");
		return 0;
	}

	// Consigne 8: Mettez le serveur en écoute en utilisant la fonction listen()
	
	if (listen(sockfd, 1) < 0)
	{
		perror("Listen failed");
		return 0;
	}

	// Consigne 9: Le serveur devrait accepter la connexion du client au serveur
	//Attention: la fonction accept() cree un nouveau File descriptor (FD) qui sera utilisé dans la suite du code pour désigner le socket du serveur
	
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_len);

	// Consigne 10: Verifiez la valeur du nouveau FD retournée par la fonction accept() 
	//et affichez un message d'erreur dans le cas ou le serveur refuse la connexion du client
	 
	if (newsockfd < 0) 
	{
		perror("Connection refused");
		return 0;
	}

	// Consigne 11: Le serveur reçoit le paquet d'initialisation du transfert de fichier, 
	// Vous pouvez utiliser la fonction recv() avec le flag MSG_WAITALL 
	init_packet ipkt;
	
	recv(newsockfd, &ipkt, sizeof(ipkt), MSG_WAITALL);
	
	// Consigne 12: Affichez un message d'erreur dans le cas où le paquet reçu ne correspond pas à un paquet d'initialisation
	// Indication: vous pouvez simplement vérifier si l'entête du paquet reçu ipkt.msg est différente de MSG_INIT
	// Dans le cas d'erreur fermez le socket (le FD résultant de l'excustion de la focntion par accept())
	
	if (ipkt.msg != 0)
	{
		perror("The package received was not an initialization package.");
		close(newsockfd);
		return 0;
	}

	// un ofstream binaire ouvert en mode out servira pour l'écriture des octets reçus par chaque fragment
	
	ofstream ofs (string(ipkt.filename).c_str(), ios::out | ios::binary);
	
	// Consigne 13: Après la réception du paquet d'initialisation du transfert des fichiers, le serveur accepte le transfert de fichier en renvoyant un 
	// message MSG_ACCEPT au client. Pensez à utiliser la fonction write() sur le nouveau socket créé par accept()
	// Remarque: write() sur un socket est équivalent à l'envoi des données
	
	MESSAGE msg = MSG_ACCEPT;

	write(newsockfd, &msg, sizeof(msg));
	
    // bytes_received est une variable pour stocker le nombre de bytes recus par le serveur 
	long long int bytes_received = 0;
	// buffer est une memoire tampon pour stocker le fragment de données reçu
	char buffer[BUFFER_SIZE];
	
	while (bytes_received != ipkt.filesize)
	{
		// On lit le stream TCP: 
		
		//Vous devriez distinguer deux cas:
		if( BUFFER_SIZE > ipkt.filesize - bytes_received) 
		{
        //Consigne 14: Cas 1: le nombre de bytes restants est inférieur à BUFFER_SIZE 
		//Vous pouvez utiliser la fonction read() sur le socket du serveur. read() sur un socket est equivalent à la reception des donnees
			n = read(newsockfd, &buffer, ipkt.filesize - bytes_received);
		}
		else 
		{
        //Consigne 15: Cas 2: le nombre de bytes restants est supérieur ou égal à BUFFER_SIZE 
		    n = read(newsockfd, &buffer, sizeof(buffer));
		}

		// Consigne 16: Écrivez les bytes reçus dans le fichier binare ofstream en utilisant ofs.write()
		
		ofs.write(buffer, n);
		
		// Le compteur du nombre de bytes reçu est incrémenté par le nombre de bytes reçus. 
		//Pensez à mettre à jour la valeur de la variable n dans chacun des cas étudié ci-haut
		bytes_received += n;
	}

	// Consign 17: Le serveur reçoit le paquet qui indique la fin du transfert des donnees. Vous pouvez utiliser la fonction recv() avec le flag MSG_WAITALL 
	
	recv(newsockfd, &ipkt, sizeof(ipkt), MSG_WAITALL);
								 
	//Consigne 18: Affichez un message d'erreur si l'entete du paquet reçu est différente MSG_END  
	
		if (ipkt.msg != 2)
	{
		perror("The package received was not an end package.");
	}

	// Consigne 19: Fermez le ofstream et le socket
	                    
	cout << "Closing socket..." << endl;
	ofs.close();
	close(newsockfd);

	return 0;
}
