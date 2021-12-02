/*
 * PWMSongParser.cpp
 *
 *  
 *      Author: 
 */

#include "PWMSongParser.h"

PWMSongParser::PWMSongParser() {
    frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
    enablePath = "/sys/devices/pwm-ctrl.42/enable0";
}

PWMSongParser::~PWMSongParser() {
    // TODO Auto-generated destructor stub
}

void PWMSongParser::tone(uint32_t frequency, uint32_t length) const
{
    std::ofstream enable(enablePath.c_str());
    std::ofstream freq(frequencyPath.c_str());
    freq << frequency;
    freq.close();
    enable << 1;
    enable.close();
    usleep(length * 1000);
    enable.open(enablePath.c_str());
    enable << 0;
    enable.close();
}
