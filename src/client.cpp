/*
 * client.cpp
 */

#include "common.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#define KEY_ESC 27

using namespace std;
using namespace cv;

const string ACCEPTED_CHARS("abcdefgABCDEFGR0123456789#");

int initClient(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;
    socklen_t addr_len;

    if (argc != 2)
        throw runtime_error("Invalid number of arguments. Usage: client <serveraddress>");

    serv_addr.sin_family = AF_INET;
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(PORT_NUMBER);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        throw runtime_error("Could not create socket");

    addr_len = sizeof(serv_addr);

    int connected = connect(sockfd, (const sockaddr *)&serv_addr, addr_len);
    if (connected < 0)
        throw runtime_error("Connection failed");

    return sockfd;
}

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
    //api->SetVariable("tessedit_char_whitelist", "abcdefgABCDEFGR1248#");
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
    /*
       for (int i = 1; i<20; i++){ 
       cout << "Testing Frame" << i << ".png" << endl;
       string fileName = "Frame" + to_string(i) + ".png";
       Mat test = imread(fileName);
       Mat gray, test2;
       cvtColor(test, gray, CV_BGR2GRAY);
       threshold(gray, test2, 127, 255, THRESH_BINARY);

       imwrite("tesseract_frame.png", test2);
       string text = readText("tesseract_frame.png");
       cout << processText(text); 
       if (processText(text) == "ABCDEFGRabcdefg012345679#") {
       cout << " (GOOD!)";
       }
       cout << endl;
       } 
       */

    int sockfd = initClient(argc, argv);

    CLIENT_MESSAGE clientMsg = {.rawData = 0};
    SERVER_MESSAGE serverMsg = {.rawData = 0};
    uint32_t LAST_PUSHB = 0;
    int n = 0;
    int captureCount = 0;
    Mat header;

    while (!clientMsg.f.QUIT)
    {
        // Receive server message
        n = recv(sockfd, &serverMsg, sizeof(SERVER_MESSAGE), MSG_WAITALL);

        if (!serverMsg.f.IDOWN)
        {
            // Receive the header
            n = recv(sockfd, &header, sizeof(Mat), MSG_WAITALL);

            // Receive data
            int frameSize = header.rows * header.cols * CV_ELEM_SIZE(header.flags);
            uchar data[frameSize];
            for (long long int bytesReceived = 0; bytesReceived != frameSize; bytesReceived += n)
            {
                int blockSize = frameSize - bytesReceived < BUFFER_SIZE ? frameSize - bytesReceived : BUFFER_SIZE;
                n = read(sockfd, data + bytesReceived, blockSize);
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
            int key = keyFlags & 0xFF;

            switch (key)
            {
                case KEY_ESC:
                    clientMsg.f.OK = 0;
                    clientMsg.f.QUIT = 1;
                    break;
                case '1': case '2': case '3': case '4':
                    clientMsg.f.RES = key - '1';
                default:
                    clientMsg.f.OK = 1;
                    break;
            }
            n = write(sockfd, &clientMsg, sizeof(CLIENT_MESSAGE));
        }
    }

    close(sockfd);
    return 0;

}
