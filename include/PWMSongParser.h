/*
 * PWMSongParser.h
 *
 *  Created on: Aug 13, 2015
 *      Author: Lucas Malo Bélanger
 */

#ifndef PWMSONGPARSER_H_
#define PWMSONGPARSER_H_

#include "SongParser.h"

/**
 * @brief 
 * 
 */
class PWMSongParser: public SongParser{
public:
    /**
     * @brief Construct a new PWMSongParser object
     * 
     */
    PWMSongParser();
    /**
     * @brief Destroy the PWMSongParser object
     * 
     */
    virtual ~PWMSongParser();

    //les chemins absolus des fichiers fréquence et run
    std::string frequencyPath;
    std::string enablePath;
private:
    //votre implémentation de la fonction tone!!!
    /**
     * @brief 
     * 
     * @param frequency 
     * @param length 
     */
    virtual void tone(uint32_t frequency, uint32_t length) const;
};

#endif /* PWMSONGPARSER_H_ */
