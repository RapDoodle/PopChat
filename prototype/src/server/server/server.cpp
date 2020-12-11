#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <ctime>

#include "server.h"
#include "utils.h"
#include "const.h"
#include "../../common/common/protocol.h"

using namespace std;

struct sockaddr_in srv;

int nMaxFd = 0;
int clients[CON_CLIENTS];

int mainSock;

int main(int argc, char** argv)
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

    /* Determine the port number */
    int port = DEFAULT_PORT;
    if (argc > 1) {
        cout << argv[1];
        port = atoi(argv[1]);
    }

    /* Call the initialize routine */
    app(port);

    // Tell the user the server is in monitor mode
}

void app(int port)
{
    /* Initialize interrupt handler */
    if (!SetConsoleCtrlHandler(intrHandler, TRUE))
        consoleLog("WARNING: Unable to setup interrupt handler");

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
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = INADDR_ANY;
    memset(&srv.sin_zero, 0, 8);

    // setsockopt
    int nOptVal = 0;
    int nOptLen = sizeof(nOptVal);
    if (setsockopt(mainSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen)) {
        cout << "Fail to call setsockopt." << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    /* Set the socket as a blocking TCP socket */
    /* 0 for blocking and 1 for non-blocking */
    u_long opt = 0;
    if (ioctlsocket(mainSock, FIONBIO, &opt) != 0)
        consoleLog("ioctlsocket call failed");

    /* Bind to local port */
    if (bind(mainSock, (struct sockaddr*)&srv, sizeof(sockaddr)) == SOCKET_ERROR) {
        consoleLog("Unable to bind to port " + to_string(port));
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        consoleLog("Successully bind to port " + to_string(port));
    }

    /* Listen for incoming requests */
    if (listen(mainSock, QUEUE_SIZE) == SOCKET_ERROR) {
        consoleLog("Unable to start listening on port " + to_string(port));
        WSACleanup();
        exit(EXIT_FAILURE);
    } else {
        consoleLog("Server started successfully");
        consoleLog("Currently listening on port " + to_string(port));
    }

    nMaxFd = mainSock;
    struct timeval tv;
    tv.tv_usec = 1;
    tv.tv_usec = 0;

    fd_set fr, fw, fe;

    /* Spin up the server */
    while (true) {

        /* File descriptors */
        FD_ZERO(&fr);   /* Read */
        FD_ZERO(&fw);   /* Write */
        FD_ZERO(&fe);   /* Exceptions */

        /* Read from the read and exception descriptor */
        FD_SET(mainSock, &fr);
        FD_SET(mainSock, &fe);

        for (int i = 0; i < CON_CLIENTS; i++) {
            if (clients[i] != 0) {
                FD_SET(clients[i], &fr);
                FD_SET(clients[i], &fe);
            }
        }

        int rtOpt = select(nMaxFd + 1, &fr, &fw, &fe, &tv);

        if (rtOpt > 0) {
            
            /* When data arrived at the port */
            if (FD_ISSET(mainSock, &fe)) {
                /* Check for exceptions */
                consoleLog("WARNING: Exception occurred");
            } else if (FD_ISSET(mainSock, &fr)) {
                /* Valid incoming request */
                int nLen = sizeof(struct sockaddr);
                int currSock = accept(mainSock, NULL, &nLen);

                if (currSock > 0) {
                    /* New socket connection */
                    int i;
                    for (i = 0; i < CON_CLIENTS; i++) {
                        if (clients[i] == 0) {
                            clients[i] = currSock;
                            send(currSock, "NACK", 5, 0);
                            break;
                        }
                    }

                    if (i == CON_CLIENTS)
                        consoleLog("WARNING: Maximum concurrent connection reached");
                }
            } else {
                for (int i = 0; i < CON_CLIENTS; i++) {
                    char recvBuff[MSG_LEN + 1];

                    if (FD_ISSET(clients[i], &fr)) {
                        if (recv(clients[i], recvBuff, MSG_LEN + 1, 0) < 0) {
                            closesocket(clients[i]);
                            clients[i] = 0;
                        } else {
                            /* handle the new message from the client */
                            if (strlen(recvBuff) > 0) {
                                cout << "[" << clients[i] << "]: " << recvBuff << endl;

                                /* Broadcast the message */
                                for (int j = 0; j < CON_CLIENTS; j++) {
                                    if (clients[j] != 0 && i != j) {
                                        try {
                                            send(clients[j], recvBuff, 140, 0);
                                        } catch (int e) {
                                            consoleLog(to_string(e));
                                        }
                                    }
                                }
                            }
                            send(clients[i], "ACK", 4, 0);
                        }
                    } 
                }
            }

        } else if (rtOpt == 0) {
            /* No connection */
        } else {
            /* Exception on the port */
            consoleLog("Error on the port");
            consoleLog(to_string(GetLastError()));
            consoleLog("Shutting down the server...");
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        /* Avoid busy waiting */
        Sleep(WAIT_T);
    }

    /* Any code below should never be executed */
}



