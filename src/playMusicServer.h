
/*
 * playMusicServer.h
 */

#ifndef PLAYMUSICSERVER_H_
#define PLAYMUSICSERVER_H_

#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <map>

class PlayMusicServer {
public:
	PlayMusicServer();
	virtual ~PlayMusicServer();

	//lecture du fichier qui contien la musique
	void readSong(std::string filename);
	
	//la musique est jouée
	virtual void play() const;

private:
	//les commandes possible
	struct command
	{
		uint32_t frequency;
		uint32_t length;
	};
	
	//la liste des commandes qui composent la musique
	std::vector<command> _commands;
protected:
	const std::vector<command>& getCommands() const;

	//la fonction tone!! Vous devez héritez de cette fonction dans PWMTest
	virtual void tone(uint32_t frequency, uint32_t length) const;
};

map<string, int> frequencies {{"A", 440}, {"A#", 466}, {"B", 494}, {"C", 523}, {"C#", 554 }, {"D", 587}, {"D#", 622}, {"E", 659}, {"F", 698}, {"F#", 734}, {"G", 784}, {"G#", 831}, {"a", 880}, {"a#", 932}, {"b", 988},
                                {"c", 1046}, {"c#", 1109}, {"d", 1175}, {"d#", 1245}, {"e", 1319}, {"f", 1397}, {"f#", 1480}, {"g", 1568}, {"g#", 1661}};
#endif /* PLAYMUSICSERVER_H_ */
