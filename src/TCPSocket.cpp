#include <netinet/in.h>    // For IPPROTO_TCP
#include <sys/socket.h>    // For SOCK_STREAM
#include <sys/types.h>

#include "DBUtl/TCPSocket.h"

using namespace DBUtl;

// constructor for TCPSocket
TCPSocket::TCPSocket()
    throw(SocketException) : CommSocket(SOCK_STREAM, IPPROTO_TCP) {
}

TCPSocket::TCPSocket(const std::string& foreignAddress, unsigned short foreignPort)
    throw(SocketException) : CommSocket(SOCK_STREAM, IPPROTO_TCP) {

    connect(foreignAddress, foreignPort);
}

TCPSocket::TCPSocket(int newConnSD) : CommSocket(newConnSD) {
}

// constructor for TCPServerSocket
TCPServerSocket::TCPServerSocket(unsigned short localPort, int queueLen)
    throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {

    setLocalPort(localPort);
    setListen(queueLen);
}

TCPServerSocket::TCPServerSocket(const std::string& localAddress,
                                 unsigned short localPort,
                                 int queueLen)
    throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {

    setLocalAddressAndPort(localAddress, localPort);
    setListen(queueLen);
}

TCPSocket* TCPServerSocket::accept() throw(SocketException) {
    int newConnSD;
    if ((newConnSD = ::accept(sockDesc, NULL, 0)) < 0) {
        throw SocketException("TCPServerSocket::accept() failed", true);
    }

    return new TCPSocket(newConnSD);
}

void TCPServerSocket::setListen(int queueLen) throw(SocketException) {
    if (listen(sockDesc, queueLen) < 0) {
        throw SocketException("TCPServerSocket::setListen failed", true);
    }
}



