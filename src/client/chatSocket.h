#pragma once
#include <winsock2.h>
#include <ctype.h>
#include <string>

#include "MessageBox.h"
#include "protocol.h"
#include "utils.h"

using namespace std;

int clientStartup(string ip, int port);
int universalVerifier(string* packet, bool showMsgBox);
string recvMsg();
string sendMsg(string msg);
bool isActive();
int packetSend(string content);
string getIpAddr(string domain);