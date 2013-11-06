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
	return ((actualTime->tm_min - 30) >= 0) ? (60 - abs(actualTime->tm_min - 30)) : abs(actualTime->tm_min - 30);
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

	for(unsigned int i = 0; i < argc; i++)
	{
		if((strcmp(argv[i], "-?") == 0) || (strcmp(argv[i], "/?") == 0))
		{
			printf("Some Kinda Help \n");
			system("pause");
			return 0;
		}

		if(strcmp(argv[i], "-f") == 0)
			DataFileName = argv[i+1];
	}

	DataParser* datas = new DataParser(DataFileName);
	
	
	while(1)
	{
		int TimeToBoom = CalculateTimeToHalfHour();
		printf("Time In Minutes to Next HalfHour: %i \n", TimeToBoom);

		if(TimeToBoom == 60)
		{
			datas->StartPlaying();
			Sleep(1000*TimeToBoom-10*60*1000);
		}else Sleep(15000);
		
	}
	return 0;
}


