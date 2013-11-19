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
    printf("Checking IP: %s %c", dest, char(13));

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

static void ScanNetwork(std::string ipAddress)
{
    std::string ip = ipAddress.substr(0, ipAddress.find_last_of('.')+1);

    WSADATA sock;
    if (WSAStartup(MAKEWORD(2,2), &sock) != 0)
        return;
    
    for(int i = 0; i < 0x20; i++)
    {
        std::string ip2 = ip;
        ip2.append(std::to_string(i));

        char address[32] = {'\0'};
        memcpy(address, ip2.c_str(), ip2.size());
        unsigned char mac[6] = {'\0'};
        unsigned char name[100] = {'\0'};

        if (get_mac(mac, address))
        {
            printf("%s : %s : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", address, (get_name(name, address)) ? (char*)name : "-", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        }
    }
    return;
}

static void ShowConnections()
{
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    IN_ADDR IPAddr;

    pIPAddrTable = (MIB_IPADDRTABLE *) malloc(sizeof(MIB_IPADDRTABLE));

    if (pIPAddrTable) 
    {
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
        {
            free(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *) malloc(dwSize);
        }
        if (pIPAddrTable == NULL) 
            return;
    }

    if ((dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 )) != NO_ERROR)
        return;

    printf("Num Entries: %ld\n", pIPAddrTable->dwNumEntries-1);

    //There Isn't any type of working connection with network (Entry 0 is LocalHost)
    if(pIPAddrTable->dwNumEntries < 2)
        return;

    for (int i=1; i < (int) pIPAddrTable->dwNumEntries; i++) 
    {
        std::string IpAddress = "";
        printf("\nInterface Index[%d]:\t%ld\n", i, pIPAddrTable->table[i].dwIndex); 
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
        IpAddress = inet_ntoa(IPAddr);
        printf("IP Address[%d]:     \t%s\n", i, inet_ntoa(IPAddr));
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwMask;
        printf("Subnet Mask[%d]:    \t%s\n", i, inet_ntoa(IPAddr));
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwBCastAddr;
        printf("BroadCast[%d]:      \t%s (%ld%)\n", i, inet_ntoa(IPAddr), pIPAddrTable->table[i].dwBCastAddr);
        printf("Reassembly size[%d]:\t%ld\n", i, pIPAddrTable->table[i].dwReasmSize);
        printf("Type and State[%d]:", i);
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_PRIMARY)
            printf("\tPrimary IP Address");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_DYNAMIC)
            printf("\tDynamic IP Address");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_DISCONNECTED)
            printf("\tAddress is on disconnected interface");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_DELETED)
            printf("\tAddress is being deleted");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_TRANSIENT)
            printf("\tTransient address");
        printf("\n\n");

        ScanNetwork(IpAddress);
    }

    if (pIPAddrTable) 
    {
        free(pIPAddrTable);
        pIPAddrTable = NULL;
    }

    system("PAUSE");
    exit(1);
    return;
}