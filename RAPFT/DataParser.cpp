#include "stdafx.h"
#include "DataParser.h"
#include "BeeperData.h"
#include <conio.h>

DataParser::DataParser(std::string DataPath)
{
	_songsAmount = 0;
	_SoundVector.clear();

	printf("DataParser: DataPath: %s \n", DataPath.c_str());
	PathName = DataPath;
    
	StartParsing();
}


DataParser::~DataParser()
{
}

void DataParser::StartParsing()
{
	std::ifstream _dataFile(PathName.c_str());
	if(!_dataFile)
	{
		printf("Can't load file - FILE DON'T EXIST! \n");
		system("pause");
		return;
	}

	if(!_dataFile.is_open())
	{
		printf("Can't load file - FILE CANNOT BE OPENED! \n");
		system("pause");
		return;
	}

	printf("!!!STARTED READING DATA!!!\r\n");

	short _linesAmount = 0;
	short _beepAmount = 0;
	std::string line;
	
	std::vector<short> freq;
	std::vector<short> mstime;
	std::vector<short> mstimeout;

	while (std::getline(_dataFile , line))
	{
		_linesAmount++;
		if(line.empty())
			continue;

		if(line.find("//") != -1)
			continue;

		std::size_t found = line.find_first_not_of("1234567890,;#-");

		if(_beepAmount != 0 && line.find("#") != -1 || line.find("----") != -1)
		{
			if(line.find("#") != -1)
				printf("Founded New Track (Track: #%s)\n", line.substr(line.find("#")+1, line.length()).c_str());

			_SoundVector.push_back(new BeeperData(PathName.c_str(), freq, mstime, mstimeout, _beepAmount, _songsAmount));
			_songsAmount++;
			_beepAmount = 0;
			if(line.find("----") != -1)
				break;
			else continue;
		}

		if (found!=std::string::npos)
			continue;

		short data[3] = {0, 0, 0};

		int pos = 0;
		int	begin = 0;
		int itr = 0;

		while((pos = line.find(',', pos+1)) && pos != line.npos)
		{
			if(pos == -1)
				continue;
			
			if(itr >= 2)
			{
				printf("CANT PARSE LINE %u WRONG AMOUNT OF DATA! \n", _linesAmount);
				break;
			}
			std::string number = line.substr(begin, pos-begin);
			begin = pos+1;
			data[itr++] = atoi(number.c_str());
			//Looking For Line End!
			if((line.find_last_of(',') == pos) && (line.find_first_of(';') == line.size()-1))
				data[itr++] = atoi(line.substr(begin, line.size()-begin).c_str());
		}
		freq.push_back(data[0]);
		mstime.push_back(data[1]);
		mstimeout.push_back(data[2]);
		_beepAmount++;
		
	}
	return;
}

void DataParser::StartPlaying()
{
	short randedsong = rand()%_songsAmount;
	printf("Started Playing %i/%i\n", randedsong, _songsAmount);
	_SoundVector[0]->play();
}