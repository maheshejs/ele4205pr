/*
 * common.h
 */

#ifndef COMMON_H
#define COMMON_H

#define BUFFER_SIZE 1024000 // bytes
#define NUMBER_OF_RESOLUTIONS 13
#define PORT_NUMBER 4099

#include<cstdint>

union MESSAGE
{
    struct
    {
        uint32_t OK     : 1;
        uint32_t QUIT   : 1;
		uint32_t RES	: 2;
    } f;
    uint32_t rawData;
};

struct tcpSockets
{
	int init;
	int comm;
};

struct res
{
    int x;
    int y;
};

const struct res USB_CAMERA_RESOLUTIONS[NUMBER_OF_RESOLUTIONS] = {
	{160, 120},
	{320, 240},
	{800, 600},
	{1280, 720},
    {176, 144},
	{320, 176},
	{352, 288},
	{432, 240},
	{864, 480},
	{960, 544},
	{960, 720},
	{1184, 656},
	{1280, 960}
};

#endif

