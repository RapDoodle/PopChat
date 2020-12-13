#pragma once
using namespace std;

#include <string>

int dbInit();
int executeQuery(string query, bool isSelect);
int createSession();
int saveMessage(int sessionId, string msg);