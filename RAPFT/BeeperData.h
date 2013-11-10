#include <vector>

#pragma once

class BeeperData
{
    public:
    BeeperData(std::string theme, std::vector<short> freq, std::vector<short> time, std::vector<short> timeout, int _BeepsAmount, short _position);
    ~BeeperData();
    void play();
    unsigned int GetPlayTime() const
    { 
        int value = 0;
        for(unsigned int i = 0; i < _BeepsAmount; i++)
            value += _mstime[i];

        return value;
    }

    unsigned int GetStopsTime() const
    {
        int value = 0;
        for(unsigned int i = 0; i < _BeepsAmount; i++)
            value += _mstimeout[i];

        return value;
    }

private:
    std::string _ThemeName;
    unsigned int _BeepsAmount;
    std::vector<short> _freq;
    std::vector<short> _mstime;
    std::vector<short> _mstimeout;
};

