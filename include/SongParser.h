/**
 * \file SongParser.h
 * \date 2 décembre 2021 
 * \author Simon Larivière et Joseph Maheshe, basé sur le code de Lucas Malo Bélanger
**/

#ifndef SONGPARSER_H_
#define SONGPARSER_H_

#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <cmath>

/**
 * \class SongParser
 * \brief SongParser est une classe fournissant des méthodes pour convertir du texte en mélodie
**/
class SongParser {
public:
    /**
     * \fn SongParser()
     * \brief Constructeur pour l'objet SongParser
    **/
    SongParser();
    /**
     * \fn ~SongParser()
     * \brief Destructeur de l'objet SongParser
    **/
    virtual ~SongParser();

    /**
     * \fn readString(std::string song)
     * \brief Fonction permettant de lire une chaîne de caractères et la convertir en commandes musicales
     * \param song
     * La chaîne de caractères à convertir en commandes
    **/
    void readString(std::string song);
    
    /**
     * \fn play()
     * \brief Fonction permettant d'exécuter les commandes musicales, faisant ainsi entendre la musique
    **/
    virtual void play() const;

private:
    uint32_t _tempo;
    
    // Les commandes possibles
    struct command
    {
        uint32_t frequency;
        uint32_t length;
    };
    
    // La liste des commandes qui composent la musique
    std::vector<command> _commands;
protected:
    /**
     * \fn getCommands()
     * \brief Fonction permettant d'obtenir la liste des commandes musicales
    **/
    const std::vector<command>& getCommands() const;

    /**
     * \fn tone(uint32_t frequency, uint32_t length)
     * \brief Fonction permettant d'émettre un son d'une durée et d'une fréquence déterminées
     * \param frequency La fréquence désirée
     * \param length La durée désirée
    **/
    virtual void tone(uint32_t frequency, uint32_t length) const;
};

#endif /* SONGPARSER_H_ */
