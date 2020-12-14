#pragma once
using namespace std;

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <string>
#include <ctime>

void consoleLog(string str);
string getCurrentTimeString();
BOOL WINAPI intrHandler(DWORD signal);
int safeToInt(string n);
string safeToSQL(string s);