#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#include <winsock2.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <ctime>

#define VERSION "0.1 beta";
#define DEFAULT_PORT 19135


int main()
{
    cout << "Client" << endl;
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