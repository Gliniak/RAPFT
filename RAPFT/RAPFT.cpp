// RAPFT.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "DataParser.h"
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

unsigned int CalculateTimeToHalfHour()
{
	time_t at = time(NULL);
	struct tm *actualTime;
	actualTime=localtime((const time_t*)&at);
	unsigned int calculated = ((actualTime->tm_min - 30) >= 0) ? (60 - abs(actualTime->tm_min - 30)) : abs(actualTime->tm_min - 30);
	return at + calculated*60-actualTime->tm_sec;
}

//It's Going To Be Useful Soon
std::string ExePath() 
{
    char buffer[MAX_PATH];
    GetModuleFileNameA( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos + 1);
}

int main(int argc, char* argv[])
{
	std::string DataFileName = "data.bps";
	int TimeInterval = -1;

	for(int i = 0; i < argc; i++)
	{
		if((strcmp(argv[i], "-?") == 0) || (strcmp(argv[i], "/?") == 0))
		{
			printf("------------- HELP -------------\n");
			printf("'-f' - Read Theme Data Based On File\n");
			printf("'-t' - Time in seconds till next play\n");
			printf("'-a' - Add Application To Autostart (UNIMPLEMENTED) \n");
			printf("'-c' - Create Copy Of Application And Run It (UNIMPLEMENTED) \n");
			printf("'-p' - DO YOU WANT TO PLAY A GAME?! \n");
			printf("------------- HELP -------------\n");
			system("pause");
			return 0;
		}

		if(strcmp(argv[i], "-f") == 0)
			DataFileName = argv[i+1];

		if(strcmp(argv[i], "-t") == 0)
			TimeInterval = atoi(argv[i+1]);
	}

	DataParser* datas = new DataParser(DataFileName);
	
	unsigned int _TTP = TimeInterval > 0 ? (time(NULL) + TimeInterval) : CalculateTimeToHalfHour();

	while(1)
	{
		int _STTP = floor(difftime(_TTP, time(NULL)));
		printf("REMAIN TIME: %i SECONDS \n", _STTP);

		if(_STTP <= 0)
		{
			datas->StartPlaying();
			_TTP = TimeInterval > 0 ? (time(NULL) + TimeInterval) : CalculateTimeToHalfHour();
		}
		else Sleep(1000);
	}
	return 0;
}


