#include "stdafx.h"
#include "DataParser.h"
#include "BeeperData.h"
#include "RAPFT.h"
#include <conio.h>
#include <dirent.h>

DataParser::DataParser(std::wstring DataPath)
{
    _songsAmount = 0;
    _SoundVector.clear();
    PathName = DataPath;
    
    StartParsing();
}


DataParser::~DataParser()
{
}

void DataParser::StartParsing()
{
    _WDIR* dir;
    struct _wdirent *ent;

    if ((dir = _wopendir (wcscmp(PathName.c_str(), GetPath().c_str()) != 0 ? PathName.c_str() : GetPath().c_str())) != NULL) 
    {
        while ((ent = _wreaddir (dir)) != NULL)
        {
            int pos = std::wstring(ent->d_name).find_last_of('.');

            if(pos == -1) // Only For Directories and Files Without dot
                continue;

            if(std::wstring(ent->d_name).substr(pos, ent->d_namlen - pos) != L".bps")
                continue;

            std::wfstream _dataFile(ent->d_name);
            if(!_dataFile)
            {
                printf("Can't load file %s - FILE DON'T EXIST! \n", ent->d_name);
                continue;
            }

            if(!_dataFile.is_open())
            {
                printf("Can't load file %s - FILE CANNOT BE OPENED! \n", ent->d_name);
                continue;
            }

            printf("!!!STARTED READING DATA!!!\r\n");
            wprintf(L"Actual Reading File: %s\n\n", ent->d_name);
            short _linesAmount = 0;
            short _beepAmount = 0;
            std::wstring line;
            std::wstring title = L"MISSINGNAME";

            std::vector<short> freq;
            std::vector<short> mstime;
            std::vector<short> mstimeout;

            while (std::getline(_dataFile , line))
            {
                _linesAmount++;
                if(line.empty() && _beepAmount == 0)
                    continue;

                if(line.find(L"//") != -1)
                    continue;

                std::size_t found = line.find_first_not_of(L"1234567890,;#-");

                if(_beepAmount != 0 && line.empty() || line.find(L"----") != -1)
                {
                    _SoundVector.push_back(new BeeperData(title, freq, mstime, mstimeout, _beepAmount, _songsAmount));
                    _songsAmount++;
                    _beepAmount = 0;
                    title = L"MISSINGNAME";
                    if(line.find(L"----") != -1)
                        break;
                    else continue;
                }

                if(line.find(L"# - ") != -1)
                    title = line.substr(line.find_first_of(L"# - ")+4, line.length());

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
                    std::wstring number = line.substr(begin, pos-begin);
                    begin = pos+1;
                    data[itr++] = _wtoi(number.c_str());
                    //Looking For Line End!
                    if((line.find_last_of(',') == pos) && (line.find_first_of(';') == line.size()-1))
                        data[itr++] = _wtoi(line.substr(begin, line.size()-begin).c_str());
                }
                freq.push_back(data[0]);
                mstime.push_back(data[1]);
                mstimeout.push_back(data[2]);
                _beepAmount++;
            }
        }
    }
    if(_songsAmount == 0)
        printf("No Songs Data Founded! \n");

    _wclosedir(dir);
    return;
}

void DataParser::StartPlaying()
{
    if(_songsAmount <= 0)
        return;

    short randedsong = rand()%_songsAmount;
    printf("\nStarted Playing %i/%i\n", randedsong, _songsAmount);
    _SoundVector[0]->play();
}