/*
 * common.h
 *
 *  Created on: May 5, 2016
 *      Author: kevin
 */

/*
 * Ce fichier devrait être intégralement fourni aux étudiants.
 * Il contient les différents types de messages et la structure de paquet d'initialisation.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

// Define essentiels pour fixer la taille maximale du nom de fichier et la taille du buffer
#define FILENAME_MAX_SIZE 	1024 		// caractères
#define BUFFER_SIZE 		1024000		// bytes

using namespace std;

// Types de message du protocole
enum MESSAGE
{
	MSG_INIT = 0,
	MSG_ACCEPT = 1,
	MSG_END = 2
};

// Structure du paquet d'initialisation du transfert de fichier
typedef struct init_packet
{
	MESSAGE msg;
	long long int filesize;
	char filename[FILENAME_MAX_SIZE];

} init_packet;

// Fonction pour connaître la taille d'un fichier en bytes
long long int getFileSize(char* filename) {
	ifstream ifs(filename, ios::in | ios::ate | ios::binary);
	long long int filesize = (long long int) ifs.tellg();
	ifs.close();
	return filesize;
}
