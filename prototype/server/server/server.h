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

#define OFFLINE		0
#define CONNECTED	1
#define CREATED		2
#define JOINING		3
#define JOINED		4

void app(int port);

struct Client{
	string ip;
	int socketId = 0;
	int roomId = NULL;
	int socketSessionId = -1;
	int status = OFFLINE;
	string nickName = "";
};

struct Room {
	int roomId = NULL;
	int sessionId = -1;
	string roomPassword;
};