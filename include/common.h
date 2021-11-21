/*
 * common.h
 */

#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#define BUFFER_SIZE 1024000
#define IMAGE_SERVER_PORT 4099
#define MUSIC_SERVER_PORT 4100

union IMAGE_CLIENT_MSG
{
    struct
    {
        uint32_t OK     : 1;
        uint32_t QUIT   : 1;
        uint32_t RES    : 2;
    } f;
    uint32_t rawData;
};

union IMAGE_SERVER_MSG
{
    struct
    {
        uint32_t READY  : 1;
        uint32_t IDOWN  : 1;
        uint32_t PUSHB  : 1;
    } f;
    uint32_t rawData;
};

#endif
