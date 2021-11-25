/*
 * SongParser.cpp
 *
 *  Created on: 
 *      
 */

#include "SongParser.h"

SongParser::SongParser() {
    // TODO Auto-generated constructor stub

}

SongParser::~SongParser() {
    // TODO Auto-generated destructor stub
}

void SongParser::readString(std::string song)
{
    static std::string letters = "abcdefgABCDEFGR";
    static std::string digits = "1248";
    static std::string tones[24] = {"A", "A#", "B", "C", "C#", "D",
                                    "D#", "E", "F", "F#", "G", "G#",
                                    "a", "a#", "b", "c", "c#", "d",
                                    "d#", "e", "f", "f#", "g", "g#"};

    _commands.clear();
    command c;

    size_t pos = 0;
    pos = song.find_first_of(letters);
    if (pos != std::string::npos)
    {
        _tempo = atoi(song.substr(0, pos).c_str());
        song.erase(0, pos);
        if (_tempo > 300)
        {
            throw std::runtime_error("Parsed tempo is too high");
        }
    }
    else
    {
        throw std::runtime_error("Failed to parse tempo");
    }

    while ((pos = song.find_first_of(digits)) != std::string::npos) {
        auto it = std::find(tones, tones + 24, song.substr(0, pos));
        if (it != std::end(tones))
            c.frequency = std::round(std::pow(2.0, std::distance(tones, it)/12.0)*440);
        else
            c.frequency = 20000; // Inaudible frequency, silence
        c.length = (60000.0/_tempo) * (4.0/atoi(song.substr(pos, pos + 1).c_str()));
        _commands.push_back(c);

        song.erase(0, pos + 1);
    }

}
const std::vector<SongParser::command>& SongParser::getCommands() const
{
    return _commands;
}
void SongParser::play() const
{
    const std::vector<command>& commands = getCommands();
    for(std::vector<command>::const_iterator it = commands.begin(); it != commands.end(); it++)
        tone(it->frequency, it->length);
}

void SongParser::tone(uint32_t frequency, uint32_t length) const
{
    std::cout << "Note On! " << frequency << " " << length <<  std::endl;
    usleep(length * 1000);

    //note if off
    std::cout << "Note Off!" << frequency << std::endl;
}
