#pragma once
using namespace std;

#include <ctime>
#include <string>
#include <stdlib.h>

#include "db.h"
#include "const.h"
#include "server.h"

int createRoom(string roomPassword);
int countOrFreeRoom(int roomId);
int groupNotify(int roomId, string message);
int groupSend(int roomId, string nickname, string message);
bool checkNameCollision(int roomId, string nickname);
string getRoomUserList(int roomId);