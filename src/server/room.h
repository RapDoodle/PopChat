#pragma once
using namespace std;

#include <string>
#include <stdlib.h>
#include <ctime>

int createRoom(string roomPassword);
int countOrFreeRoom(int roomId);
int groupSend(int roomId, string nickname, string message);
bool checkNameCollision(int roomId, string nickname);