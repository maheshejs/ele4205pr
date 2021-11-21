/*
 * mainMusicServer.cpp
 *
 *  
 *      Author: 
 */

#include "PWMSongParser.h"
int main(int argc, char **argv)
{
    //remplacez SongParser par PWMSongParser!!!
    //SongParser met les actions note on, note off ainsi que la fréquence dans l'invite de commande avec les timings précis
    PWMSongParser parser;
    //SongParser parser;

    // Gamme de sol
    //parser.readString("80G4a8b8c8e8f#8g4f#8e8d8c8b8a8G4R4g4R4G1");
    //Autumn leaves
    parser.readString("120R8E8F#8G8c2R8D8E8F#8b2R8C8D8E8a2R8B8C#8D#8G2R8E8F#8G8c2R8D8E8F#8b2R8C8D8E8a2R8F#8a8G8E2R4D#8E8F#8B8F#4R8F#8E8F#8G2R8G8F#8G8a2R8D8d8c8b2r4a#8b8c8c8a8a8F#4R8c8b4b2R8E8a4a8G8F#4G8B4E1");
    //On joue la musique
    parser.play();
    return 0;
}
