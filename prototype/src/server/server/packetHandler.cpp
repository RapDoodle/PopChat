#include <string>
#include <iostream>
#include <winsock2.h>

#include "packetHandler.h"
#include "protocol.h"
#include "server.h"
#include "utils.h"
#include "room.h"

using namespace std;

int packetSend(int client, string content)
{
    string packet = packetWrapper(content);
    send(client, packet.c_str(), packet.length(), 0);
    return 0;
}

int packetHandler(struct Client* client, char* buff)
{
    string str(buff);

    /* The string for searching (will be modified) */
    string srchStr = buff;
    string version = nextParam(&srchStr);

    if (version != PROTOCOL_VERSION) {
        packetSend(client->socketId, PACKET_TYPE_SUCCESS " Incompatible protocol version");
        return -1;
    }

    string checkSum = nextParam(&srchStr);
    string calCheckSum = to_string(calculateCheckSum(srchStr));
    if (checkSum != calCheckSum) {
        packetSend(client->socketId, PACKET_TYPE_VALIDATION_ERROR);
        return -1;
    }

    string type = nextParam(&srchStr);
    int typeInt;
    try {
        typeInt = stoi(type);
    } catch (const std::invalid_argument&) {
        typeInt = -1;
    }
    cout << type << endl;
    /* Route different packets to different routine */
    if (type == PACKET_TYPE_PING) {
        /* Ping */
        cout << "Ping!" << type << endl;
        packetSend(client->socketId, PACKET_TYPE_PONG);
    } else if (type == PACKET_TYPE_CREATE_ROOM) {
        /* Create room */
        if (client->roomId != NULL) {
            packetSend(client->socketId, PACKET_TYPE_ERROR " You've already joined room " + 
                to_string(client->roomId));
            return -1;
        }
        string pwd = nextParam(&srchStr);
        client->roomId = createRoom(pwd);
        client->nickName = nextParam(&srchStr);
        packetSend(client->socketId, PACKET_TYPE_CLIENT_ACK " Joined successfully");
    } else if (type == PACKET_TYPE_JOIN_ROOM) {
        /* Join the room */
        string roomId = nextParam(&srchStr);

        if (client->roomId != NULL && client->roomId != safeToInt(roomId)) {
            packetSend(client->socketId, PACKET_TYPE_ERROR " You've already another room " +
                to_string(client->roomId));
            return -1;
        }
        
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i].roomId == safeToInt(roomId)) {
                string pwd = nextParam(&srchStr);
                if (pwd != rooms[i].roomPassword) {
                    packetSend(client->socketId, PACKET_TYPE_ERROR " Incorrect password");
                    return -1;
                }
                client->roomId = rooms[i].roomId;
                packetSend(client->socketId, PACKET_TYPE_SUCCESS " Joined successfully");
            }
        }
    } else if (type == PACKET_TYPE_CLIENT_SEND) {
        cout << client->roomId << endl;
        if (client->roomId == NULL) {
            packetSend(client->socketId, PACKET_TYPE_ERROR " You are not in any room");
            return -1;
        }
        for (int i = 0; i < CON_CLIENTS; i++) {
            if (onlineClients[i].socketId != client->socketId && onlineClients[i].roomId == client->roomId) {
                packetSend(onlineClients[i].socketId, PACKET_TYPE_SERVER_SEND " <" + client->nickName + "> " + srchStr);
            }
        }
        packetSend(client->socketId, PACKET_TYPE_SERVER_ACK);
    } else {
        packetSend(client->socketId, PACKET_TYPE_ERROR);
        return -1;
    }

    return 0;
}
