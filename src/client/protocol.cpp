#include "protocol.h"

using namespace std;

int calculateCheckSum(string s)
{
    /* They do not need to repair a damaged packet. It
     * has been guaranteed by lower layer protocols. We
     * just need an efficient way to check the validity
     * of the packet. */
    int checkSum = 0;
    for (int i = 0; i < s.length(); i++) {
        checkSum += (int)s[i];
        if (i % 50 == 0) {
            checkSum %= 100;
        }
    }
    return checkSum;
}

string nextParam(string* srchStr)
{
    /* Get the next parameter separated by the default
     * delimiter. The srchStr will be modified to contain
     * the string without the parameter */
    size_t pos = srchStr->find(DELIMITER);
    string val = srchStr->substr(0, pos);
    srchStr->erase(0, pos + DELIMITER_SIZE);
    return val;
}

string packetWrapper(string content)
{
    /* Adds the protocol version number and a checksum
     * to the packet */
    string wrapped = PROTOCOL_VERSION;
    wrapped = wrapped + DELIMITER +
        to_string(calculateCheckSum(content)) +
        DELIMITER + content;
    return wrapped;
}