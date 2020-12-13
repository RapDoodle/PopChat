#pragma once
using namespace std;

#include "server.h"

int packetSend(int client, string content);
int packetHandler(struct Client* client, char* buff);