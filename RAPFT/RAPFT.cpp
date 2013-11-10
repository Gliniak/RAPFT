// RAPFT.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "DataParser.h"
#include "RAPFT.h"
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

BOOL IsUserAdmin()
{
    BOOL isAdmin;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup; 
    isAdmin = AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup);

    if(isAdmin) 
    {
        if (!CheckTokenMembership( NULL, AdministratorsGroup, &isAdmin)) 
            isAdmin = FALSE;
 
        FreeSid(AdministratorsGroup); 
    }
    return(isAdmin);
}

void AddProgramToAutoStart(bool Allusers)
{
    TCHAR szPath[MAX_PATH];
    DWORD pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);

    HKEY newValue;
    if (RegOpenKey(Allusers ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &newValue) != ERROR_SUCCESS)
    {
        printf("Can't Add To Autostart! Shit Happened! \n");
        return;
    }
    DWORD pathLenInBytes = pathLen * sizeof(*szPath);
    if (RegSetValueEx(newValue, TEXT("They See Me Rollin' They Hatin"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS)
    {
        printf("Can't Set To Autostart! Shit Happened! \n");
        RegCloseKey(newValue);
        return;
    }
    printf("Successfully Added AutoStart Code! \n");
    RegCloseKey(newValue);
}

std::string CopyToLocation(std::string _location)
{
    std::string path;
    path.append(GetPath().c_str());
    path.append(GetExeName().c_str());

    _location.append(GetExeName().c_str());

    if(path == _location)
        return "TRUE";

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
            printf("'-a' - Add Application To Autostart\n");
            printf("'-c' - Create Copy Of Application And Run It \n");
            printf("'-p' - DO YOU WANT TO PLAY A GAME?! \n");
            printf("------------- HELP -------------\n");
            system("pause");
            return 0;
        }

        if(strcmp(argv[i], "-c") == 0)
        {
            std::string AppLocation = CopyToLocation(argv[i+1]);

            if(AppLocation != "TRUE") // nice wayout is nice! :D
            {
                PROCESS_INFORMATION pi = {0};
                STARTUPINFOA         si = {0};

                si.cb = sizeof(STARTUPINFOA);

                CreateProcessA(AppLocation.c_str(), NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return 0;
            }
        }

        if(strcmp(argv[i], "-a") == 0)
            AddProgramToAutoStart(IsUserAdmin() ? true : false);

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


