#pragma once
using namespace std;

#include "server.h"

#include <winsock2.h>
#include <iostream>
#include <string>

#include "packetHandler.h"
#include "protocol.h"
#include "utils.h"
#include "room.h"
#include "db.h"

int packetSend(int client, string content);
int packetHandler(struct Client* client, char* buff);