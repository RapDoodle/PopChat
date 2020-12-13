#pragma once
using namespace std;

int clientStartup(string ip, int port);
int universalVerifier(string packet, bool showMsgBox);
string recvMsg();
string sendMsg(string msg);
bool isActive();
int packetSend(string content);