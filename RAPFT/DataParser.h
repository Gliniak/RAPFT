#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "BeeperData.h"

class DataParser
{
    public:
        DataParser(std::wstring);
        ~DataParser();

        void StartParsing();
        void StartPlaying();
        short GetAmountOfSongs() { return _songsAmount; }

    private:
        std::wstring PathName;
        short _songsAmount;
        std::vector<BeeperData*> _SoundVector;
};

