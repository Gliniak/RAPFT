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

std::string GetPath() 
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos + 1);
}

std::string GetExeName() 
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of( "\\/" );
	return std::string(buffer).substr(pos + 1, std::string(buffer).length() - pos);
}

std::string CopyToLocation(std::string _location)
{
	std::string path;
	path.append(GetPath().c_str());
	path.append(GetExeName().c_str());

	_location.append(GetExeName().c_str());

	if(CopyFileA(path.c_str(), LPCSTR(_location.c_str()), FALSE) == 0)
		printf("COPYING FILE STATUS: FAILED! ERROR CODE: %u \n", GetLastError());
	else 
	{
		printf("Copying Exe To New Location FINISHED. NEW LOCATION: %s\n", _location.c_str());
		return _location;
	}
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
			printf("'-c' - Create Copy Of Application And Run It \n");
			printf("'-p' - DO YOU WANT TO PLAY A GAME?! \n");
			printf("------------- HELP -------------\n");
			system("pause");
			return 0;
		}

		if(strcmp(argv[i], "-c") == 0)
		{
			std::string asd = CopyToLocation(argv[i+1]);

			PROCESS_INFORMATION pi = {0};
			STARTUPINFOA         si = {0};

			si.cb = sizeof(STARTUPINFOA);

			CreateProcessA(asd.c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
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
		printf("I'M A' FIRIN' MAH LAZER IN: %i SECONDS \n", _STTP);

		if(_STTP <= 0)
		{
			datas->StartPlaying();
			_TTP = TimeInterval > 0 ? (time(NULL) + TimeInterval) : CalculateTimeToHalfHour();
		}
		else Sleep(1000);
	}
	return 0;
}


