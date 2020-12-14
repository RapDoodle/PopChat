#include <winsock2.h>
#include <iostream>
#include <string>

#include "packetHandler.h"
#include "protocol.h"
#include "server.h"
#include "utils.h"
#include "room.h"
#include "db.h"

using namespace std;

int packetSend(int client, string content)
{
    string packet = packetWrapper(content);
    cout << "[DEBUG] " << packet << endl;
    return send(client, packet.c_str(), packet.length(), 0);
}

int packetHandler(struct Client* client, char* buff)
{
    string str(buff);

    /* The string for searching (will be modified) */
    string srchStr = buff;
    string version = nextParam(&srchStr);

    if (version != PROTOCOL_VERSION) {
        packetSend(client->socketId, PACKET_TYPE_ERROR " Incompatible protocol version");
        return -1;
    }

    string checkSum = nextParam(&srchStr);
    string calCheckSum = to_string(calculateCheckSum(srchStr));
    if (checkSum != calCheckSum) {
        packetSend(client->socketId, PACKET_TYPE_VALIDATION_ERROR);
        return -1;
    }

    string type = nextParam(&srchStr);

    /* Route different packets to different routine */
    if (type == PACKET_TYPE_PING) {
        /* Ping */
        packetSend(client->socketId, PACKET_TYPE_PONG);

    } else if (type == PACKET_TYPE_CREATE_ROOM) {
        /* Create room */
        if (client->roomId != NULL) {
            packetSend(client->socketId, PACKET_TYPE_ERROR " You've already joined room " + 
                to_string(client->roomId));
            return -1;
        }
        string pwd = nextParam(&srchStr);
        int roomId = createRoom(pwd);
        string nickname = nextParam(&srchStr);

        /* Make sure the user does not use the bot's name */
        if (nickname == BOT_NAME) {
            packetSend(client->socketId, PACKET_TYPE_FAILED " The nickname already exists in the room. Please pick another name.");
            return -1;
        }

        client->roomId = roomId;
        client->nickname = nickname;
        client->socketSessionId = createSocketSession(client->ip, client->nickname);

        /* Admitted procedure */
        packetSend(client->socketId, PACKET_TYPE_ADMITTED " " + to_string(client->roomId) + " " + client->nickname);
        client->status = CREATED;
        

    } else if (type == PACKET_TYPE_JOIN_ROOM) {
        /* Join the room */
        string roomId = nextParam(&srchStr);

        if (client->roomId != NULL && client->roomId != safeToInt(roomId)) {
            packetSend(client->socketId, PACKET_TYPE_FAILED " You've already joined another room " +
                to_string(client->roomId));
            return -1;

        }
        
        bool found = false;

        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i].roomId == safeToInt(roomId)) {
                found = true;
                string pwd = nextParam(&srchStr);
                if (pwd != rooms[i].roomPassword) {
                    packetSend(client->socketId, PACKET_TYPE_FAILED " Incorrect password");
                    return -1;
                }

                /* Check for name collision */
                string nickname = nextParam(&srchStr);
                if (checkNameCollision(rooms[i].roomId, nickname)) {
                    packetSend(client->socketId, PACKET_TYPE_FAILED " The nickname already exists in the room. Please pick another name.");
                    return -1;
                }

                /* Admitted procedure */
                client->roomId = rooms[i].roomId;
                client->nickname = nickname;
                client->status = JOINING;
                client->socketSessionId = createSocketSession(client->ip, client->nickname);
                packetSend(client->socketId, PACKET_TYPE_ADMITTED " " + to_string(client->roomId) + " " + client->nickname);

            }
        }

        if (!found) {
            packetSend(client->socketId, PACKET_TYPE_FAILED " Room number not found. Consider creating one?");
            return -1;

        }

    } else if (type == PACKET_TYPE_LEAVE_ROOM) {
        string nickname = client->nickname;
        int roomId = client->roomId;
        client->roomId = NULL;
        client->nickname = "";
        // packetSend(client->socketId, PACKET_TYPE_SUCCESS " You've left the room successfully");
        int clientCount = groupSend(roomId, BOT_NAME, client->nickname + " has left the chat.");
        if (clientCount <= 0)
            countOrFreeRoom(roomId);

    } else if (type == PACKET_TYPE_JOINED) {
        cout << "hi" << endl;
        if (client->status == CREATED) {
            for (int i = 0; i < MAX_ROOMS; i++) {
                if (rooms[i].roomId == client->roomId) {
                    packetSend(client->socketId, PACKET_TYPE_SERVER_SEND " Bot Created successfully. Room number: " +
                        to_string(rooms[i].roomId) + ". Room password: " + rooms[i].roomPassword);
                    break;
                }
            }
            client->status = JOINED;

        } else if (client->status == JOINING) {
            packetSend(client->socketId, PACKET_TYPE_SERVER_SEND " Bot You've joined room " + to_string(client->roomId));
            client->status = JOINED;

        } else {
            packetSend(client->socketId, PACKET_TYPE_SERVER_SEND " Bot Welcome back to room " + to_string(client->roomId));
            client->status = JOINED;

        }

    } else if (type == PACKET_TYPE_CLIENT_SEND) {
        if (client->roomId == NULL) {
            packetSend(client->socketId, PACKET_TYPE_ERROR " You are not in any room");
            return -1;
        }

        groupSend(client->roomId, client->nickname, srchStr);

        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i].roomId == client->roomId) {
                int chatSessionId = rooms[i].sessionId;
                if (chatSessionId != -1)
                    saveMessage(chatSessionId, client->socketSessionId, srchStr);
                break;
            }
        }

    } else {
        packetSend(client->socketId, PACKET_TYPE_ERROR);
        return -1;

    }

    return 0;
}
