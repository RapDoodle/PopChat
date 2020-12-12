#pragma once
using namespace std;

#include <Windows.h>

void consoleLog(string str);
string getCurrentTimeString();
BOOL WINAPI intrHandler(DWORD signal);
int safeToInt(string n);