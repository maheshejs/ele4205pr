/*
 *  PWMSongParser.cpp
 *
 *  Date    : 2 décembre 2021
 *  Auteurs : Simon Larivière et Joseph Maheshe
 */

#include "PWMSongParser.h"

PWMSongParser::PWMSongParser() {
    _frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
    _enablePath = "/sys/devices/pwm-ctrl.42/enable0";
}

PWMSongParser::~PWMSongParser() {
    // TODO Auto-generated destructor stub
}

void PWMSongParser::tone(uint32_t frequency, uint32_t length) const
{
    std::ofstream enable(_enablePath.c_str());
    std::ofstream freq(_frequencyPath.c_str());
    freq << frequency;
    freq.close();
    enable << 1;
    enable.close();
    usleep(length * 1000);
    enable.open(_enablePath.c_str());
    enable << 0;
    enable.close();
}
