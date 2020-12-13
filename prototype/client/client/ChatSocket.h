using namespace std;

string clientStartup(string ip, int port);
string recvMsg();
string sendMsg(string msg);
bool isActive();
int packetSend(string content);