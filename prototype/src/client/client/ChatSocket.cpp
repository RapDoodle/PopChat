#include <winsock2.h>
#include <string>

#include "protocol.h"

using namespace std;

int sock;
struct sockaddr_in srv;
WSADATA ws;
bool active = false;
int errCount = 0;

#define MAX_ATTEMP 100

string clientStartup(string ip, int port)
{
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        WSACleanup();
        return "Failed to connect";
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock < 0) {
        WSACleanup();
        return "Fail to open the socket";
    }

    /* Set as an blocking socket */
    u_long mode = 0;
    if (ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR) {
        return "ioctlsocket call failed";
    }

    char charIp[64];
    strcpy(charIp, ip.c_str());

    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(charIp);
    memset(&srv.sin_zero, 0, 8);

    if (connect(sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        WSACleanup();
        return "Failed to connect";
    }
    active = true;
    return "Connected";
}

int packetSend(string content)
{
    string packet = packetWrapper(content);
    if (active)
        return send(sock, packet.c_str(), packet.length() + 1, 0);
    else
        return -1;
}

bool isActive() {
    return active;
}

string recvMsg()
{
    char buff[256] = {};
    int rtOpt = recv(sock, buff, 256, 0);
    if (rtOpt > 0) {
        /* Reset the error counter */
        errCount = 0;
        string buffStr = buff;
        return buffStr;
    } else {
        errCount++;
        if (errCount >= MAX_ATTEMP) {
            active = false;
            closesocket(sock);
            WSACleanup();
            return packetWrapper(PACKET_TYPE_ERROR " Lost connection with the server");
        }
        return "";
    }

}

string sendMsg(string msg)
{
    int msgLen = msg.size();

    if (!active)
        return "You are not connected to any server";

    if (msgLen <= 0)
        return "";

    if (msgLen > 140)
        return "Message too long";

    if (packetSend(PACKET_TYPE_CLIENT_SEND " " + msg) == SOCKET_ERROR) {
        return "Failed to send";
    } else {
        // return msg;
        return "";  // Wait for the server to return the message
    }
}