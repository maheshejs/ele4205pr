/*
 * mainMusicServer.cpp
 *
 *  
 *      Author: 
 */

#include "PWMSongParser.h"
#include "TCPServer.h"
#include "common.h"

int main(int argc, char **argv)
{
    TCPServer musicServer(MUSIC_SERVER_PORT);
    musicServer.initSocket();
    // Receive the string size
    int size;
    recv(musicServer.getCommSocket(), &size, sizeof(int), MSG_WAITALL);

    // Receive song
    char song[size];
    recv(musicServer.getCommSocket(), song, size, MSG_WAITALL);

    PWMSongParser parser;
    parser.readString(std::string(song));
    parser.play();

    /*
    PWMSongParser parser;
    parser.readString("120R8E8F#8G8c2R8D8E8F#8b2R8C8D8E8a2R8B8C#8D#8G2R8E8F#8G8c2R8D8E8F#8b2R8C8D8E8a2R8F#8a8G8E2R4D#8E8F#8B8F#4R8F#8E8F#8G2R8G8F#8G8a2R8D8d8c8b2r4a#8b8c8c8a8a8F#4R8c8b4b2R8E8a4a8G8F#4G8B8E1");
    parser.play();
    */
    return 0;
}
