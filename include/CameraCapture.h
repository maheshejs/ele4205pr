/**
 *  \file CameraCapture.h 
 *
 *  \date 2 décembre 2021
 *  \author Simon Larivière et Joseph Maheshe
 */
#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <opencv2/opencv.hpp>

#define NUMBER_OF_RESOLUTIONS 13

/**
 * \struct resolution
 * \brief Résolution d'une image
 * 
 */
struct resolution
{
    int x; /*!< Largeur */
    int y; /*!< Hauteur*/
};

const resolution USB_CAMERA_RESOLUTIONS[NUMBER_OF_RESOLUTIONS] = {
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

/**
 * \brief CameraCapture est une classe héritant de cv::VideoCapture qui ouvre la caméra USB pour les captures vidéo
 * 
 */
class CameraCapture : public cv::VideoCapture
{
public:
    /**
     * \brief Constructeur pour l'objet CameraCapture
     * 
     */
    CameraCapture();

    /**
     * \brief Fonction permettant permettant d'attribuer une résolution à la caméra USB
     * 
     * \param ID_RESOLUTION La résolution à attribuer
     */
    void setResolution(uint32_t ID_RESOLUTION);
};

#endif
