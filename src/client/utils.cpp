#include <iostream>
#include <chrono>
#include <string>
#include <ctime>

using namespace std;

string getCurrentDateTimeString()
{
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm ptm;
    localtime_s(&ptm, &now);
    char strBuf[21];
    strftime(strBuf, sizeof(strBuf), "%Y-%m-%d %H:%M:%S", &ptm);
    return strBuf;
}

string getCurrentTimeString()
{
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm ptm;
    localtime_s(&ptm, &now);
    char strBuf[21];
    strftime(strBuf, sizeof(strBuf), "%H:%M", &ptm);
    return strBuf;
}

int safeToInt(string n)
{
    try {
        return stoi(n);
    } catch (const std::invalid_argument&) {
        return -1;
    }
}

string safeToSQL(string s)
{
    int len = s.length();
    for (int i = 0; i <= len; i++) {
        if (s[i] == '\'' || s[i] == '\\') {
            s.insert(i, "\\");
            len++;
            i++;
        }
    }
    return s;
}