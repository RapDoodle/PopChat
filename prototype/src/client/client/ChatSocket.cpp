#include <winsock2.h>
#include <string>

using namespace std;

int sock;
struct sockaddr_in srv;
WSADATA ws;
bool active = false;

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

bool isActive() {
    return active;
}

string recvMsg()
{
    char buff[140 + 1] = {};
    int rtOpt = recv(sock, buff, 141, 0);
    if (rtOpt > 0) {
        string buffStr = buff;
        if (buffStr == "NACK") {
            /* Connected */
            return "";
        } else if (buffStr == "ACK")
            return "";
        return buffStr;
    } else {
        return "Unable to connect";
    }

}

string sendMsg(string msg)
{
    int msgLen = msg.size();

    if (msgLen <= 0)
        return "";

    if (msgLen > 140)
        return "Message too long";

    char charStr[1024];
    strcpy(charStr, msg.c_str());

    if (send(sock, charStr, 140, 0) == SOCKET_ERROR) {
        return "Failed to send";
    } else {
        return msg;
    }
}