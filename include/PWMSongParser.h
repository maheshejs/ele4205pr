/**
 *  \file PWMSongParser.h
 *
 *  \date 2 décembre 2021
 *  \author Simon Larivière et Joseph Maheshe, basé sur le code de Lucas Malo Bélanger
 */

#ifndef PWMSONGPARSER_H_
#define PWMSONGPARSER_H_

#include "SongParser.h"

/**
 * \brief PWMSongParser est une classe héritant de SongParser qui surchage une méthode pour jouer les notes de musique avec le PWM
 * 
 */
class PWMSongParser: public SongParser{
public:
    /**
     * \brief   Constructeur de l'objet PWMSongParser qui initialise les chemins des fichier pour configurer la fréquence et l'activation du PWM.
     *          Quant au rapport cyclique, il est toujours forcé manuellement à 512 pour avoir une onde carrée.
     */
    PWMSongParser();
    /**
     * \brief Destructeur de l'objet PWMSongParser
     * 
     */
    virtual ~PWMSongParser();

private:
    /**
     * \brief Fonction permettant de traduire une fréquence et une durée en une note musicale jouée par le PWM
     * 
     * \param frequency La fréquence de note à traduire
     * \param length    La durée de note à traduire
     */
    virtual void tone(uint32_t frequency, uint32_t length) const;

    std::string _frequencyPath;
    std::string _enablePath;
};

#endif /* PWMSONGPARSER_H_ */
