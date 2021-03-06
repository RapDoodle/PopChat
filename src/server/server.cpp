using namespace std;

#include "server.h"

struct sockaddr_in srv;

int nMaxFd = 0;
struct Client onlineClients[CON_CLIENTS];
struct Room rooms[MAX_ROOMS];

int mainSock;

int main(int argc, char** argv)
{
    /* Verify the number of arguments */
    if (argc % 2 == 0) {
        if (argc == 2) {
            string param = argv[1];
            if (param == "-h" || param == "--help") {
                cout << "server -p [port number] --dbu [username] -dbp [password] --dbport [db port number]" << endl;
                exit(0);
            }
        }
        cout << "Invalid number of parameters. Use server.exe --help for more information." << endl;
        exit(EXIT_FAILURE);
    }

    /* Determine the port number */
    int port = DEFAULT_PORT;
    string dbUsername = "root";
    string dbPassword = "";
    int dbPort = 3306;

    int i = 1;
    while (i < argc) {
        string param = argv[i];

        if (param == "-p" || param == "--port") {
            port = atoi(argv[i + 1]);
        } else if (param == "--dbu") {
            dbUsername = argv[i + 1];
        } else if (param == "--dbp") {
            dbPassword = argv[i + 1];
        } else if (param == "--dbport") {
            dbPort = atoi(argv[i + 1]);
        } else {
            cout << "Invalid parameter. Check for manual using server.exe -h for more information.";
            exit(EXIT_FAILURE);
        }
        i += 2;
    }

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

    string versionInfo = "Pop Chat Server v" SRV_VERSION;
    consoleLog(versionInfo);

    /* Initialize the database */
    if (dbInit(dbUsername, dbPassword, dbPort) < 0) {
        exit(EXIT_FAILURE);
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

        /* Bind file descriptors to sockets */
        for (int i = 0; i < CON_CLIENTS; i++) {
            if (onlineClients[i].socketId != 0) {
                FD_SET(onlineClients[i].socketId, &fr);
                FD_SET(onlineClients[i].socketId, &fe);
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
                SOCKADDR_IN addr;
                char ip[INET_ADDRSTRLEN];

                /* Accept connection from the main (master) socket */
                int currSock = accept(mainSock, (SOCKADDR*)&addr, &nLen);
                inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
                string ipStr = ip;
                consoleLog("Received connection from " + ipStr);

                if (currSock > 0) {
                    /* New socket connection */
                    int i;
                    for (i = 0; i < CON_CLIENTS; i++) {
                        if (onlineClients[i].socketId == 0) {
                            onlineClients[i].socketId = currSock;
                            onlineClients[i].ip = ip;
                            packetSend(currSock, PACKET_TYPE_SUCCESS DELIMITER "Connected");
                            break;
                        }
                    }

                    if (i == CON_CLIENTS)
                        consoleLog("WARNING: Maximum concurrent connection reached");
                }

            } else {
                for (int i = 0; i < CON_CLIENTS; i++) {
                    char recvBuff[PACKET_MAX_SIZE];

                    if (FD_ISSET(onlineClients[i].socketId, &fr)) {
                        if (recv(onlineClients[i].socketId, recvBuff, PACKET_MAX_SIZE, 0) < 0) {
                            int roomId = onlineClients[i].roomId;
                            string nickname = onlineClients[i].nickname;
                            closesocket(onlineClients[i].socketId);
                            consoleLog("Socket " + to_string(onlineClients[i].socketId) + " disconnected");
                            onlineClients[i].socketId = 0;
                            onlineClients[i].roomId = NULL;
                            onlineClients[i].socketSessionId = -1;
                            onlineClients[i].status = OFFLINE;
                            onlineClients[i].nickname = "";
                            int count = groupSend(roomId, BOT_NAME, nickname + " has disconnected.");
                            if (count <= 0) {
                                countOrFreeRoom(roomId);
                            } else {
                                string userList = getRoomUserList(roomId);
                                groupNotify(roomId, PACKET_TYPE_USER_LIST DELIMITER "1" DELIMITER "1" DELIMITER + userList);
                            }

                        } else {
                            /* Handle the new message from the client */
                            if (strlen(recvBuff) > 0) {
                                // cout << "[" << onlineClients[i].socketId << "][" << onlineClients[i].roomId << "][" << onlineClients[i].nickname << "]: " << recvBuff << endl;
                                packetHandler(&onlineClients[i], recvBuff);

                            }
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



