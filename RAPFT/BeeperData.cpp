#include "stdafx.h"
#include "BeeperData.h"
#include <Windows.h>

BeeperData::BeeperData(std::wstring name, std::vector<short> freq, std::vector<short> time, std::vector<short> timeout, int BeepsAmount, short _position)
{
    _freq = freq;
    _mstime = time;
    _mstimeout = timeout;

    _BeepsAmount = BeepsAmount;
    printf("--------------- THEME #%u SUMMARY --------------------\n", _position+1);
    printf("---- Theme Number: %u\n", _position+1);
    wprintf(L"---- Theme Name: %ws\n", name.c_str());
    printf("---- Amount of Sound Calls: %u\n", BeepsAmount);
    printf("---- Play Time: %u\n", GetPlayTime()+GetStopsTime());
    printf("--------------- END OF SUMMARY ----------------------- \n\n");
}


BeeperData::~BeeperData(void)
{
}

void BeeperData::play()
{
    for(unsigned int i = 0; i < _BeepsAmount; i++)
    {
        printf("%i/%i - Playing Sound: %u Time: %i\n", i+1, _BeepsAmount, _freq[i], _mstime[i]);
        Beep(_freq[i], _mstime[i]);
        Sleep(_mstimeout[i]);
    }
}