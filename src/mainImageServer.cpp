/**
 * \file mainImageServer.cpp
 * \date 2 décembre 2021 
 * \author Simon Larivière et Joseph Maheshe, basé sur le code de Lucas Malo Bélanger
**/

#include "ImageServer.h"
#include "CameraCapture.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 \mainpage
 Cette application client-serveur permet de capturer des images côté serveur et les envoyer au client via le protocole TCP/IP. Le client affiche ces images et peut
 demander quatre résolutions différentes à l'aide des touches 1 à 4 du clavier. 
 
 Les images envoyées au client sont censées contenir du texte représentant de la musique sous forme codifiée. Lorsque l'on appuie sur un bouton côté serveur, le client 
 extrait le texte signifiant, corrige si nécessaire quelques erreurs et renvoie le texte au serveur pour celui-ci fasse jouer la musique.
**/
int main(int argc, char *argv[])
{
    ImageServer imgServer(IMAGE_SERVER_PORT);
    imgServer.initSocket();
    CameraCapture capture;

    IMAGE_CLIENT_MSG clientMsg = {.rawData = 0};
    IMAGE_SERVER_MSG serverMsg = {.rawData = 0};
    uint32_t CURRENT_RES = 0;
    uint32_t LAST_PUSHB = 0;
    int n = 0;
    Mat frame;

    while (!clientMsg.f.QUIT)
    {
        // Send the server message
        serverMsg = imgServer.computeMessage();
        n = write(imgServer.getCommSocket(), &serverMsg, sizeof(IMAGE_SERVER_MSG));

        if (serverMsg.f.PUSHB && !LAST_PUSHB) {
            if (fork() == 0)
            {
                execl("./mainMusicServer", "mainMusicServer", (char *)0);
                return 0;
            }
        }
        LAST_PUSHB = serverMsg.f.PUSHB;
        if (!serverMsg.f.IDOWN)
        {
            capture >> frame;

            if (frame.empty())
                throw runtime_error("Failed to capture image");

            // Send the header
            n = write(imgServer.getCommSocket(), &frame, sizeof(Mat));

            // Send the data
            int frameSize = frame.total() * frame.elemSize();

            for (long long int bytesSent = 0; bytesSent != frameSize; bytesSent += n)
            {
                int blockSize = frameSize - bytesSent < BUFFER_SIZE ? frameSize - bytesSent : BUFFER_SIZE;
                n = write(imgServer.getCommSocket(), frame.data + bytesSent, blockSize);
            }

            n = recv(imgServer.getCommSocket(), &clientMsg, sizeof(IMAGE_CLIENT_MSG), MSG_WAITALL);

            if (clientMsg.f.RES != CURRENT_RES)
                capture.setResolution(CURRENT_RES = clientMsg.f.RES);
        }
    }
    return 0;
}
