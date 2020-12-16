#include "db.h"
#include "room.h"
#include "const.h"
#include "server.h"

using namespace std;

int createRoom(string roomPassword)
{
	for (int i = 0; i < MAX_ROOMS; i++) {
		if (rooms[i].roomId == NULL) {
			srand(time(NULL));
			int roomId = rand() % 100000 + 10000;

			/* Check for collision */
			for (int j = 0; j < MAX_ROOMS; j++) {
				if (rooms[j].roomId == roomId) {
					/* Collision detected */
					return createRoom(roomPassword);
				}
			}

			rooms[i].roomId = roomId;
			rooms[i].roomPassword = roomPassword;
			rooms[i].sessionId = createChatSession();
			return rooms[i].roomId;
		}
	}
		
	/* No empty room */
	return -1;
}

int countOrFreeRoom(int roomId)
{
	if (roomId <= 0) {
		return 0;
	}

	/* Check for the number of users in the room */
	int count = 0;
	for (int i = 0; i < CON_CLIENTS; i++) {
		if (onlineClients[i].roomId == roomId) {
			count++;
		}
	}

	if (count == 0) {
		/* Find the room in the array and free the entry */
		for (int i = 0; i < MAX_ROOMS; i++) {
			if (rooms[i].roomId == roomId) {
				rooms[i].roomId = NULL;
				rooms[i].sessionId = -1;
				rooms[i].roomPassword = "";
				consoleLog("Room " + to_string(roomId) + " has been recycled.");
				return 0;
			}
		}
	}
	
	return count;
}

int groupSend(int roomId, string nickname, string message)
{
	if (roomId == NULL) {
		return 0;
	}

	int count = 0;
	for (int i = 0; i < CON_CLIENTS; i++) {
		if (onlineClients[i].roomId == roomId) {
			packetSend(onlineClients[i].socketId, PACKET_TYPE_SERVER_SEND " " + nickname + " " + message);
			count++;
		}
	}

	/* Exited normally */
	return count;
}

bool checkNameCollision(int roomId, string nickname)
{
	/* Avoid collision with the bot */
	if (nickname == BOT_NAME) {
		return true;
	}

	for (int i = 0; i < CON_CLIENTS; i++) {
		if (onlineClients[i].roomId == roomId && onlineClients[i].nickname == nickname) {
			return true;
		}
	}
	
	/* When no name collision is found */
	return false;
}