#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>
#include <string.h>

static unsigned int CalculateTimeToHalfHour()
{
    time_t at = time(NULL);
    struct tm *actualTime;
    actualTime=localtime((const time_t*)&at);
    unsigned int calculated = ((actualTime->tm_min - 30) >= 0) ? (60 - abs(actualTime->tm_min - 30)) : abs(actualTime->tm_min - 30);
    return at + calculated*60-actualTime->tm_sec;
}

static std::string GetPath() 
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos + 1);
}

static std::string GetExeName() 
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of( "\\/" );
    return std::string(buffer).substr(pos + 1, std::string(buffer).length() - pos);
}