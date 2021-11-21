/*
 * SongParser.h
 *
 *  
 *      Author: 
 */

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

class SongParser {
public:
    SongParser();
    virtual ~SongParser();

    //lecture du fichier qui contient la musique
    void readString(std::string song);
    
    //la musique est jouée
    virtual void play() const;

private:
        uint32_t _tempo;
    //les commandes possibles
    struct command
    {
        uint32_t frequency;
        uint32_t length;
    };
    
    //la liste des commandes qui composent la musique
    std::vector<command> _commands;
protected:
    const std::vector<command>& getCommands() const;

    //la fonction tone!! Vous devez héritez de cette fonction dans PWMTest
    virtual void tone(uint32_t frequency, uint32_t length) const;
};

#endif /* SONGPARSER_H_ */
