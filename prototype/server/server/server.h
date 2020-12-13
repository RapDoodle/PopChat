#pragma once
#include "const.h"

#ifndef GLOBAL_ONLINE_CLIENTS
#define GLOBAL_ONLINE_CLIENTS
extern struct Client onlineClients[CON_CLIENTS];
#endif

#ifndef GLOBAL_ONLINE_ROOMS
#define GLOBAL_ONLINE_ROOMS
extern struct Room rooms[MAX_ROOMS];
#endif


void app(int port);

struct Client{
	string ip;
	int socketId = 0;
	int roomId = NULL;
	int socketSessionId = -1;
	int status = 0;
	string nickName = "";
};

struct Room {
	int roomId = NULL;
	int sessionId = -1;
	string roomPassword;
};