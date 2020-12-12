#pragma once
#include "const.h"

#define SRV_VERSION "1.0"

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
	int socketId = 0;
	int roomId = NULL;
	int status = 0;
	string nickName = "";
};

struct Room {
	int roomId = NULL;
	string roomPassword;
};