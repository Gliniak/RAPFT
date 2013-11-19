#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <locale>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

static unsigned int CalculateTimeToHalfHour()
{
    time_t at = time(NULL);
    struct tm *actualTime;
    actualTime=localtime((const time_t*)&at);
    unsigned int calculated = ((actualTime->tm_min - 30) >= 0) ? (60 - abs(actualTime->tm_min - 30)) : abs(actualTime->tm_min - 30);
    return unsigned int(at + calculated*60-actualTime->tm_sec);
}

static std::wstring GetPath() 
{
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos + 1);
}

static std::wstring GetExeName() 
{
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(pos + 1, std::wstring(buffer).length() - pos);
}


static bool get_name(unsigned char* name, char dest[32])
{
    struct in_addr destip;
    struct hostent* info;

    destip.s_addr = inet_addr(dest);

    info = gethostbyaddr((char*)&destip, 4, AF_INET);

    if (info != NULL)
    {
        strcpy((char*)name, info->h_name);
    }
    else
    {
        return false;
    }

    return true;
}

static bool get_mac(unsigned char* mac , char dest[32])
{
    in_addr destip;
    ULONG mac_address[2];
    ULONG mac_address_len = 6;

    destip.s_addr = inet_addr(dest);

    SendARP(IPAddr(destip.S_un.S_addr), 0, mac_address, &mac_address_len);

    if (mac_address_len)
    {
        BYTE* mac_address_buffer = (BYTE*)&mac_address;
        for (int i = 0; i < (int)mac_address_len; i++)
        {
            mac[i] = (char)mac_address_buffer[i];
        }
        return true;
    }
    return false;
}

static void ShowDataConnectionStats()
{
    PMIB_TCPSTATS stats;
    stats = (MIB_TCPSTATS*) malloc(sizeof(MIB_TCPSTATS));

    GetTcpStatistics(stats);
    printf("Active Connections: %u\n", stats->dwActiveOpens);
    return;
}