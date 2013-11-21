// RAPFT.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "DataParser.h"
#include "RAPFT.h"

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

int _tmain(int argc, TCHAR* argv[])
{
    //I can't Force Unicode in any other way
    setlocale(LC_CTYPE,"");

    std::wstring DataPath = GetPath();
    int TimeInterval = -1;

    for(int i = 0; i < argc; i++)
    {
        if((wcscmp(argv[i], L"-?") == 0) || (wcscmp(argv[i], L"/?") == 0))
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

        if(wcscmp(argv[i], L"-c") == 0)
        {
            std::wstring AppLocation = CopyToLocation(argv[i+1]);

            if(wcscmp(AppLocation.c_str(), GetFullPath().c_str()) != 0)
            {
                PROCESS_INFORMATION pi = {0};
                STARTUPINFOW         si = {0};

                si.cb = sizeof(STARTUPINFOW);

                std::wstring arguments = L"";
                for(int j = 1; j < argc; j++)
                {
                    if(j == i || j == i+1)
                        continue;

                    arguments.append(argv[j]);
                    arguments.push_back(' ');
                }

                CreateProcess(AppLocation.c_str(), LPWSTR(arguments.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi); 
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                exit(1);
            }
        }

        if(wcscmp(argv[i], L"-a") == 0)
            AddProgramToAutoStart(IsUserAdmin() ? true : false);

        if(wcscmp(argv[i], L"-f") == 0)
            DataPath = argv[i+1];

        if(wcscmp(argv[i], L"-t") == 0)
            TimeInterval = _wtoi(argv[i+1]);

        if(wcscmp(argv[i], L"-p") == 0)
            ShowConnections();
    }

    DataParser* datas = new DataParser(DataPath);
    
    unsigned int _TTP = TimeInterval > 0 ? unsigned int(time(NULL) + TimeInterval) : CalculateTimeToHalfHour();

    while(1)
    {
        int _STTP = int(floor(difftime(_TTP, time(NULL))));
        
        //Let's Print It Only In One Line
        printf("%cI'M A' FIRIN' MAH LAZER IN: %i SECONDS", char(13), _STTP);

        if(_STTP <= 0)
        {
            if(datas->GetAmountOfSongs() == 0)
                datas->StartParsing();

            datas->StartPlaying();
            _TTP = TimeInterval > 0 ? unsigned int(time(NULL) + TimeInterval) : CalculateTimeToHalfHour();
        }
        else Sleep(1000);
    }
    return 0;
}


