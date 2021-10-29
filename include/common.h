/*
 * common.h
 */

#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#define BUFFER_SIZE 1024000
#define PORT_NUMBER 4099

union CLIENT_MESSAGE
{
    struct
    {
        uint32_t OK     : 1;
        uint32_t QUIT   : 1;
        uint32_t RES    : 2;
    } f;
    uint32_t rawData;
};

union SERVER_MESSAGE
{
    struct
    {
        uint32_t READY  : 1;
        uint32_t IDOWN  : 1;
        uint32_t PUSHB  : 1;
    } f;
    uint32_t rawData;
};

struct tcpSockets
{
    int init;
    int comm;
};

#endif
