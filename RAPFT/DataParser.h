#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "BeeperData.h"

class DataParser
{
	public:
		DataParser(std::string);
		~DataParser();

		void StartParsing();
		void StartPlaying();

	private:
		std::string PathName;
		short _songsAmount;
		std::vector<BeeperData*> _SoundVector;
};

