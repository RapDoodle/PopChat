#include "ChatSocket.h"

int sock;
struct sockaddr_in srv;
bool active = false;
int errCount = 0;

#define MAX_ATTEMP 100

int clientStartup(string host, int port)
{
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        WSACleanup();
        msgBoxCritical("Unable to initialize the network socket.");
        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock < 0) {
        WSACleanup();
        msgBoxCritical("Fail to open the socket.");
        return -1;
    }

    /* Set as an blocking socket */
    u_long mode = 0;
    if (ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR) {
        WSACleanup();
        msgBoxCritical("ioctlsocket call failed.");
        return -1;
    }

    char charHost[64];
    strcpy(charHost, host.c_str());

    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    if (isdigit(charHost[0])) {
        srv.sin_addr.s_addr = inet_addr(charHost);
    } else {
        struct hostent* hent = gethostbyname(host.c_str());
        if (hent == NULL) {
            msgBoxCritical("Host not found.");
            WSACleanup();
            return -1;
        }
        int i = 0;
        while (hent->h_addr_list[i] != 0) {
            srv.sin_addr.s_addr = *(u_long*)hent->h_addr_list[i++];
        }
    }
    
    memset(&srv.sin_zero, 0, 8);

    if (connect(sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        WSACleanup();
        msgBoxCritical("Unable to connect. Please verify the host and the port number.");
        return -1;
    }

    string packet = recvMsg();
    int type = universalVerifier(&packet, true);

    if (type < 0) {
        return -1;
    } else if (type != safeToInt(PACKET_TYPE_SUCCESS) || nextParam(&packet) != "Connected") {
        closesocket(sock);
        WSACleanup();
        msgBoxCritical("The connection is not established. Please try again later.");
        return -1;
    }

    // getIpAddr("uic.edu.hk");

    active = true;
    return 0;
}

int universalVerifier(string* packet, bool showMsgBox) {
    string version = nextParam(packet);

    if (version != PROTOCOL_VERSION) {
        if (showMsgBox)
            msgBoxCritical("Unable to establish communication with the server. Please use the version of PopChat supported by the server.");
        return -1;
    }
    string checkSum = nextParam(packet);

    string calCheckSum = to_string(calculateCheckSum(*packet));
    if (checkSum != calCheckSum) {
        if (showMsgBox)
            msgBoxCritical("Packet error. Please try again later.");
        return -1;
    }

    string type = nextParam(packet);

    return safeToInt(type);
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
            return packetWrapper(PACKET_TYPE_ERROR DELIMITER "Lost connection with the server");
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

    if (packetSend(PACKET_TYPE_CLIENT_SEND DELIMITER + msg) == SOCKET_ERROR) {
        return "Failed to send";
    } else {
        // return msg;
        return "";  // Wait for the server to return the message
    }
}

string getIpAddr(string domain)
{
    struct hostent* hent;
    struct in_addr addr;
    
    hent = gethostbyname("uic.edu.hk");

    //if (hent == NULL) {
    //    msgBoxCritical("Host not found. Please verify the domain");
    //    return "";
    //}

    /* Read out the ip address as type string */
    int i = 0;
    string ipStr = "";
    while (hent->h_addr_list[i] != 0) {
        addr.s_addr = *(u_long*)hent->h_addr_list[i++];
        /* inet_ntoa converts IP address to ASCII string with dotted-decimal format */
        ipStr += inet_ntoa(addr); 
    }

    msgBoxInfo(QString::fromStdString(ipStr));
    return ipStr;
}