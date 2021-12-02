/**
 * \file mainClient.cpp
 * \date 2 décembre 2021 
 * \author Simon Larivière et Joseph Maheshe
**/

#include "ImageClient.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace std;
using namespace cv;

/**
 * \var ACCEPTED_CHARS
 * \brief Chaîne de caractères contenant les caractères acceptés pour la lecture
**/
const string ACCEPTED_CHARS = "abcdefgABCDEFGR0123456789#";

/**
 * \fn readText(string fileName)
 * \brief Fonction permettant de lire du texte dans un fichier image
 * N.B.: Code fortement inspiré de https://tesseract-ocr.github.io/tessdoc/APIExample.html
 * \param fileName
 * Le chemin d'accès relatif du fichier image
 * \return string La chaîne de caractère extraite de l'image
**/
string readText(string fileName)
{
    // 
    char* outText;

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix* image = pixRead(fileName.c_str());
    api->SetVariable("tessedit_char_whitelist", ACCEPTED_CHARS.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    string output(outText);

    // Destroy used object and release memory
    api->End();
    delete api;
    delete[] outText;
    pixDestroy(&image);

    return output;
}

/**
 * \fn processText(string text)
 * \brief Fonction permettant d'élaguer les caractères non voulus et corriger certaines erreurs possibles de lecture
 * \param text
 * La chaîne de caractère à traiter
 * \return string La chaîne de caractère traitée
**/
string processText(string text)
{
    string result = "";
    for (char const &c: text) {
        if (ACCEPTED_CHARS.find(c) != string::npos)
        {
            result += c;
        }
    }
    // Fix 8's converted to B's
    for (int i = 1; i < result.size(); i++)
    {
        if (result[i] == 'B' && (result[i-1] <= 'z' && result[i-1] >= 'A') || result[i-1] == '#') 
        {
            result[i] = '8';
        }
    }
    // Fix D's converted to 0's
    for (int i = 3; i < result.size(); i++)
    {
        if (result[i] == '0' && (result[i-1] <= '9' && result[i-1] >= '0'))
        {
            result[i] = 'D';
        }
    }
    // Fix a's converted to 3's
    for (int i = 3; i < result.size(); i++)
    {
        if (result[i] == '3' && (result[i-1] <= '9' && result[i-1] >= '0'))
        {
            result[i] = 'a';
        }
    }
    // Fix G's converted to 6's
    for (int i = 2; i < result.size(); i++)
    {
        if (result[i] == '6' && (result[i-1] <= '9' && result[i-1] >= '0'))
        {
            result[i] = 'G';
        }
    }
    
    return result;
}

/**
 * \mainpage 
**/
int main(int argc, char *argv[])
{
    if (argc != 2)
        throw runtime_error("Invalid number of arguments. Usage: client <serveraddress>");

    ImageClient imgClient(IMAGE_SERVER_PORT, string(argv[1]));
    imgClient.initSocket();

    IMAGE_CLIENT_MSG clientMsg = {.rawData = 0};
    IMAGE_SERVER_MSG serverMsg = {.rawData = 0};
    uint32_t LAST_PUSHB = 0;
    int n = 0;
    int captureCount = 0;
    Mat header;

    while (!clientMsg.f.QUIT)
    {
        // Receive server message
        n = recv(imgClient.getCommSocket(), &serverMsg, sizeof(IMAGE_SERVER_MSG), MSG_WAITALL);

        if (!serverMsg.f.IDOWN)
        {
            // Receive the header
            n = recv(imgClient.getCommSocket(), &header, sizeof(Mat), MSG_WAITALL);

            // Receive data
            int frameSize = header.rows * header.cols * CV_ELEM_SIZE(header.flags);
            uchar data[frameSize];
            for (long long int bytesReceived = 0; bytesReceived != frameSize; bytesReceived += n)
            {
                int blockSize = frameSize - bytesReceived < BUFFER_SIZE ? frameSize - bytesReceived : BUFFER_SIZE;
                n = read(imgClient.getCommSocket(), data + bytesReceived, blockSize);
            }
            Mat frame(header.rows, header.cols, header.type(), data);

            if (serverMsg.f.PUSHB && !LAST_PUSHB)
            {
                ++captureCount;
                if (fork() == 0)
                {
                    // Save frame to PNG
                    string fileName = "Frame" + to_string(captureCount) + ".png";
                    imwrite(fileName.c_str(), frame);

                    // Preprocess frame with simple thresholding
                    Mat gray, thresh;
                    cvtColor(frame, gray, CV_BGR2GRAY);
                    threshold(gray, thresh, 127, 255, THRESH_BINARY);

                    // OCR followed by text processing
                    imwrite("TesseractFrame.png", thresh);
                    string song = processText(readText("TesseractFrame.png"));
                    cout << song << endl; 

                    // Send song size followed by string song
                    int size = song.size();
                    TCPClient musicClient(MUSIC_SERVER_PORT, string(argv[1]));
                    musicClient.initSocket();
                    n = write(musicClient.getCommSocket(), &size, sizeof(int));
                    n = write(musicClient.getCommSocket(), song.c_str(), size);
                    return 0;
                }
            }

            LAST_PUSHB = serverMsg.f.PUSHB;

            // Display video
            imshow("Frame", frame);

            int keyFlags = waitKey(30);

            clientMsg = imgClient.computeMessage(keyFlags & 0xFF);
            n = write(imgClient.getCommSocket(), &clientMsg, sizeof(IMAGE_CLIENT_MSG));
        }
    }
    return 0;
}
