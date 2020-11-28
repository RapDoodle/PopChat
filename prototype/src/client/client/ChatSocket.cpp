#include "pch.h"
#include <winsock.h>
#include <string>

#define PORT 19135

int sock;
struct sockaddr_in srv;
WSADATA ws;

using namespace System::Runtime::InteropServices;

System::String^ clientStartup()
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

    /* Set as an non-blocking socket */
    u_long mode = 1;
    if (ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR) {
        return "ioctlsocket call failed";
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&srv.sin_zero, 0, 8);

    connect(sock, (struct sockaddr*)&srv, sizeof(srv));

    return "";

    //if (connect(sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
    //    // WSACleanup();
    //    return "Failed to connect";
    //} else {
    //    return "Connected";
    //}
}

System::String^ recvMsg()
{
    char buff[140 + 1] = {};
    int rtOpt = recv(sock, buff, 141, 0);
    if (rtOpt > 0) {
        System::String^ buffStr = gcnew System::String(buff);
        if (buffStr == "NACK")
            return "Connected\n";
        else if (buffStr == "ACK")
            return "";
        return buffStr;
    } else {
        return "";
    }
    
}

System::String^ sendMsg(System::String^ msg)
{
    int msgLen = msg->Length;
    
    if (msgLen <= 0)
        return "Empty message is not allowed";

    if (msgLen > 140)
        return "Message too long";

    char* charStr = (char*)Marshal::StringToHGlobalAnsi(msg).ToPointer();
    if (send(sock, charStr, 140, 0) == SOCKET_ERROR) {
        return "Failed to send";
    } else {
        return msg;
    }
}