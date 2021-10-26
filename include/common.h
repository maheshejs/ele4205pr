/*
 * common.h
 */

#pragma once

#define BUFFER_SIZE  1024000 // bytes
#define NUMBER_OF_RESOLUTIONS 13
struct timing {
    int resX;
    int resY;
    double fps;
};

struct timing usbCameraTimings[NUMBER_OF_RESOLUTIONS] = {
        {176, 144, 30.0}, {160, 120, 30.0}, {320, 176, 30.0}, {320, 240, 30.0},
        {352, 288, 30.0}, {432, 240, 30.0}, {800, 600, 30.0}, {864, 480, 30.0},
        {960, 544, 30.0}, {960, 720, 30.0}, {1184, 656, 30.0}, {1280, 720, 30.0},
        {1280, 960, 30.0}
};
