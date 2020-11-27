#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <ctime>

#define VERSION "0.1 beta";
#define DEFAULT_PORT 19135
#define QUEUE_SIZE 32

using namespace std;

int app();
void consoleLog(string str);
string getCurrentTimeString();

struct sockaddr_in srv;
fd_set fr, fw, fe;

int mainSock;

int main()
{
    cout << endl;
    cout << "    _____               _____ _           _    " << endl;
    cout << "   |  __ \\             / ____| |         | |  " << endl;
    cout << "   | |__) |__  _ __   | |    | |__   __ _| |_ " << endl;
    cout << "   |  ___/ _ \\| '_ \\  | |    | '_ \\ / _` | __|" << endl;
    cout << "   | |  | (_) | |_) | | |____| | | | (_| | |_ " << endl;
    cout << "   |_|   \\___/| .__/   \\_____|_| |_|\\__,_|\\__|" << endl;
    cout << "              | |                             " << endl;
    cout << "              |_|                             " << endl;
    cout << endl;

    string versionInfo = "Pop Chat Server v" VERSION;
    consoleLog(versionInfo);
    app();
}

int app()
{
    /* Initialize the WSA variables for Windows */
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        consoleLog("Failed to perform WSAStartup");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        consoleLog("Successful WSAStartup");
    }

    /* Initialize the socket */
    mainSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mainSock == INVALID_SOCKET) {
        consoleLog("Failed to open the master socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        consoleLog("Main socket initialized");
    }

    /* Initialize the env for sockaddr structure */
    srv.sin_family = AF_INET;
    srv.sin_port = htons(DEFAULT_PORT);
    srv.sin_addr.s_addr = INADDR_ANY;
    memset(&srv.sin_zero, 0, 8);

    /* Set the socket as a non-blocking TCP socket */
    /* 0 for blocking and 1 for non-blocking */
    u_long opt = 1;
    if (ioctlsocket(mainSock, FIONBIO, &opt) != 0)
        consoleLog("ioctlsocket call failed");

    /* Bind to local port */
    if (bind(mainSock, (struct sockaddr*)&srv, sizeof(sockaddr)) == SOCKET_ERROR) {
        consoleLog("Unable to bind to port 19135");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        consoleLog("Successully bind to port 19135");
    }

    /* Listen for incoming requests */
    if (listen(mainSock, QUEUE_SIZE) == SOCKET_ERROR) {
        consoleLog("Unable to start listening on port 19135");
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        consoleLog("Server started successfully");
        consoleLog("Currently listening on port 19135");
    }

    consoleLog("Server shutting down...");
    WSACleanup();
    return 0;
}

void consoleLog(string str)
{
    cout << "[" << getCurrentTimeString() << "] " << str << endl;
}

string getCurrentTimeString()
{
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm ptm;
    localtime_s(&ptm, &now);
    char strBuf[21];
    strftime(strBuf, sizeof(strBuf), "%Y-%m-%d %H:%M:%S", &ptm);
    return strBuf;
}

