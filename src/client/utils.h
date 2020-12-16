#pragma once
using namespace std;

#include <iostream>
#include <chrono>
#include <string>
#include <ctime>

string getCurrentDateTimeString();
string getCurrentTimeString();
int safeToInt(string n);
string safeToSQL(string s);
string nextLine(string* text);