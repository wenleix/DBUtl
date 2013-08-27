#include "DBUtl/Socket.h"
#include "DBUtl/SocketException.h"
#include "DBUtl/CommUtils.h"

#include <arpa/inet.h>         // For inet_addr()
#include <netdb.h>             // For gethostbyname()
#include <netinet/in.h>        // For sockaddr_in
#include <sys/types.h>         // For data types
#include <sys/socket.h>        // For socket(), connect(), send(), and recv()
#include <unistd.h>            // For close()

#include <cstring>            // For memset

using namespace DBUtl;

// constructor
Socket::Socket(int type, int protocol) throw(SocketException) {
    // Make a new socket
    if ((sockDesc = socket(PF_INET, type, protocol)) < 0) {
        throw SocketException("Socket creation failed ", true);
    }
}

Socket::Socket(int sockDesc) {
    this->sockDesc = sockDesc;
}

// destructor
Socket::~Socket() {
    ::close(sockDesc);  // call close() from unistd.h
    sockDesc = -1;
}

std::string Socket::getLocalAddress() throw(SocketException) {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t*) &addr_len) < 0) {
        throw SocketException("Get socket name failed", true);
    }

    return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort() throw(SocketException) {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t*) &addr_len) < 0) {
        throw SocketException("Get socket name failed", true);
    }

    return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort) throw(SocketException) {
    // Bind the socket to its port
    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(localPort);

    if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
        throw SocketException("Set local port failed with bind()", true);
    }
}

void Socket::setLocalAddressAndPort(const std::string &localAddress,
                                    unsigned short localPort)
            throw(SocketException) {

    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));

    Utils::fillAddr(localAddress, localPort, localAddr);

    if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
        throw SocketException("Set local address and port failed with bind()", true);
    }
}
