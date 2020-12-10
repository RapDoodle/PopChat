#pragma once
#include <Windows.h>

using namespace std;

void consoleLog(string str);
string getCurrentTimeString();
BOOL WINAPI intrHandler(DWORD signal);