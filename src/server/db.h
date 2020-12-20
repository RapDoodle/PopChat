#pragma once
using namespace std;

#include <string>
#include <stdlib.h>
#include <iostream>

int dbInit(string username, string password, int port);
int executeQuery(string query, bool isSelect);
int createChatSession();
int createSocketSession(string ip, string nickname);
int saveMessage(int chat_session_id, int socket_session_id, string msg);