#include <stdlib.h>
#include <ctime>

#include "room.h"
#include "const.h"
#include "server.h"

using namespace std;

int createRoom(string roomPassword)
{
	// TO-DO: Check for any collision
	for (int i = 0; i < MAX_ROOMS; i++) {
		if (rooms[i].roomId == NULL) {
			srand(time(NULL));
			rooms[i].roomId = rand() % 100000 + 10000;
			rooms[i].roomPassword = roomPassword;
			return rooms[i].roomId;
		}
	}
		
	/* No empty room */
	return -1;
}