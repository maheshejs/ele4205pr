void readSong(std::string song)
{
    std::string data1,data2,data3, data4, data5, frequency, length, delay;
    _commands.clear();

    command c;

    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (file.fail()) {
        std::cout << "Failed to open "+filename << std::endl;
        exit(-1);
    }	
    while(!file.eof())
    {
        file >> data1 >> data2 >> data3;
        file.ignore(1);

        file >> data4 >> data5;
        file.ignore(1);

        frequency.assign(data2, 10,3);
        length.assign(data3,7,3);
        delay.assign(data5, 0, 3);
        c.frequency = atoi(frequency.c_str());
        c.delay = atoi(delay.c_str());
        c.length = atoi(length.c_str());
        _commands.push_back(c);
    } 
} 
const std::vector<SongParser::command>& SongParser::getCommands() const {
    return _commands;
}
void SongParser::play() const
{
    const std::vector<command>& commands = getCommands();
    
    std::ofstream duty((PWM_PATH + DUTY).c_str());
    duty << 512;
    duty.close();

    for(std::vector<command>::const_iterator it = commands.begin(); it != commands.end(); it++)
    {
        tone(it->frequency,it->length);
        usleep((it->delay - it->length) * 1000);
    }
}

void SongParser::tone(uint32_t frequency, uint32_t length) const
{
    std::cout << "Note On! " << frequency << std::endl;
    std::ofstream enable((PWM_PATH + ENABLE).c_str());
    std::ofstream freq((PWM_PATH + FREQ).c_str());
    freq << frequency;
    freq.close();
    enable << 1;
    enable.close();
    
    usleep(length);
    
    enable.open((PWM_PATH + ENABLE).c_str());
    enable << 0;
    enable.close();
}
