#pragma once
using namespace std;

#include <string>
#include <iostream>
#include <winsock2.h>

#include "db.h"
#include "room.h"
#include "utils.h"
#include "server.h"
#include "protocol.h"
#include "packetHandler.h"

int packetSend(int client, string content);
int packetHandler(struct Client* client, char* buff);