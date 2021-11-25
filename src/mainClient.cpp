/*
 * mainClient.cpp
 */

#include "ImageClient.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace std;
using namespace cv;

const string ACCEPTED_CHARS = "abcdefgABCDEFGR0123456789#";

string readText(string fileName)
{
    // Code fortement inspiré de https://tesseract-ocr.github.io/tessdoc/APIExample.html
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
                // Save image and read text if button is pushed
                if (fork() == 0)
                {
                    string fileName = "Frame" + to_string(captureCount) + ".png";
                    imwrite(fileName.c_str(), frame);
                    Mat gray, thresh;
                    cvtColor(frame, gray, CV_BGR2GRAY);
                    threshold(gray, thresh, 127, 255, THRESH_BINARY);
                    imwrite("tesseract_frame.png", thresh);
                    string song = processText(readText("tesseract_frame.png"));
                    cout << song; 
                    cout << endl;
                    // Autumn Leaves
                    // string song = "120R8E8F#8G8c2R8D8E8F#8b2R8C8D8E8a2R8B8C#8D#8G2R8E8F#8G8c2R8D8E8F#8b2R8C8D8E8a2R8F#8a8G8E2R4D#8E8F#8B8F#4R8F#8E8F#8G2R8G8F#8G8a2R8D8d8c8b2R4a#8b8c8c8a8a8F#4R8c8b4b2R8E8a4a8G8F#4G8B4E1";
                    // Smoke on the water
                    // string song = "144C8R8D#8R8F4F8C8R8D#8R8F#8F2C8R8D#8R8F4F8D#8R8C1";
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
