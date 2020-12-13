#pragma once
#include <string>
using namespace std;

#define DELIMITER " "
#define DELIMITER_SIZE 1
#define PROTOCOL_VERSION "POP/1.0"

#define PACKET_TYPE_PING				"00"
#define PACKET_TYPE_PONG				"01"
#define PACKET_TYPE_SERVER_ACK			"02"
#define PACKET_TYPE_CLIENT_ACK			"03"
#define PACKET_TYPE_SUCCESS				"04"
#define PACKET_TYPE_FAILED				"05"
#define PACKET_TYPE_VALIDATION_ERROR	"06"
#define PACKET_TYPE_DISCONNECT			"07"
#define PACKET_TYPE_DISCONNECT_ACK		"08"
#define PACKET_TYPE_ERROR				"09"
#define PACKET_TYPE_CREATE_ROOM			"10"
#define PACKET_TYPE_JOIN_ROOM			"11"
#define PACKET_TYPE_REJOIN_ROOM			"12"
#define PACKET_TYPE_LEAVE_ROOM			"13"
#define PACKET_TYPE_ADMITTED			"14"
#define PACKET_TYPE_JOINED				"15"
#define PACKET_TYPE_REQUEST_USER_LIST	"16"
#define PACKET_TYPE_USER_LIST			"17"
#define PACKET_TYPE_CLIENT_SEND			"20"
#define PACKET_TYPE_SERVER_SEND			"21"
#define PACKET_TYPE_CLIENT_REQUEST		"22"

int calculateCheckSum(string s);
string nextParam(string* srchStr);
string packetWrapper(string content);