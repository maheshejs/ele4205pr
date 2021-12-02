/**
 *  \file mainMusicServer.cpp
 *
 *  \date 2 décembre 2021
 *  \author Simon Larivière et Joseph Maheshe
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

    return 0;
}
