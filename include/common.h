/*
 * common.h
 */

#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#define BUFFER_SIZE 1024000
#define IMAGE_SERVER_PORT 4099
#define MUSIC_SERVER_PORT 4100


/**
 * \union IMAGE_CLIENT_MSG
 * \brief Format de message du client d'images
 */
union IMAGE_CLIENT_MSG
{
    struct
    {
        uint32_t OK     : 1; /*!< Requête d'une image au serveur*/
        uint32_t QUIT   : 1; /*!< Arrêt de communication avec le serveur*/
        uint32_t RES    : 2; /*!< Résolution de l'image*/
    } f;
    uint32_t rawData;
};

/**
 * \union IMAGE_SERVER_MSG
 * \brief Format de message du serveur d'images
 */
union IMAGE_SERVER_MSG
{
    struct
    {
        uint32_t READY  : 1; /*!< La luminosité du milieu est correcte et le bouton-poussoir n'est pas appuyé*/
        uint32_t IDOWN  : 1; /*!< La luminosité du milieu est faible*/
        uint32_t PUSHB  : 1; /*!< La luminosité du milieu est correcte et le bouton-poussoir est pas appuyé*/
    } f;
    uint32_t rawData;
};

#endif
