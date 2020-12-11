#pragma once
#define DELIMITER " "
#define DELIMITER_SIZE 1
#define VERSION "POP/1.0"

using namespace std;

int calculateCheckSum(string s);
string nextParam(string* srchStr);
string packetWrapper(string content);