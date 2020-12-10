#include <iostream>
#include <chrono>
#include <string>
#include <ctime>

using namespace std;

#include "utils.h"

BOOL WINAPI intrHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        consoleLog("Server shutting down...");
        WSACleanup();
    }
    exit(0);
}

void consoleLog(string str)
{
    cout << "[" << getCurrentTimeString() << "] " << str << endl;
}

string getCurrentTimeString()
{
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm ptm;
    localtime_s(&ptm, &now);
    char strBuf[21];
    strftime(strBuf, sizeof(strBuf), "%Y-%m-%d %H:%M:%S", &ptm);
    return strBuf;
}