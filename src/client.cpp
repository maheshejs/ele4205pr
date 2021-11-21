/*
 * client.cpp
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
                    string text = readText("tesseract_frame.png");
                    cout << processText(text); 
                    if (processText(text) == "ABCDEFGRabcdefg012345679#") {
                        cout << " (GOOD!)";
                    }
                    cout << endl;
                    return 0;
                }
            }

            LAST_PUSHB = serverMsg.f.PUSHB;

            // Display video
            imshow("Frame", frame);

            int keyFlags = waitKey(30);

            imgClient.computeMessage(keyFlags & 0xFF);
            clientMsg = imgClient.getMessage();
            n = write(imgClient.getCommSocket(), &clientMsg, sizeof(IMAGE_CLIENT_MSG));
        }
    }
    return 0;
}
