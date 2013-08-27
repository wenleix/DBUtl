#include "DBUtl/CommSocket.h"
#include "DBUtl/CommUtils.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

using namespace DBUtl;

// constructor
CommSocket::CommSocket(int type, int protocol)
    throw(SocketException) : Socket(type, protocol) {
}

CommSocket::CommSocket(int newConnSD) : Socket(newConnSD) {
}

void CommSocket::connect(const std::string& foreignAddress,
                         unsigned short foreignPort)
               throw(SocketException) {
    // Get the address of the requested host.
    sockaddr_in destAddr;

    Utils::fillAddr(foreignAddress, foreignPort, destAddr);

    // Try to connect to the given port.
    // call connect() from socket.h
    if (::connect(sockDesc, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
        throw SocketException("CommSocket: connect failed", true);
    }
}

void CommSocket::send(const void* buffer, uint64_t bufferLen)
               throw(SocketException) {
    // call send() from socket.h
    if (::send(sockDesc, (raw_type *) buffer, bufferLen, 0) < 0) {
        throw SocketException("CommSocket: send failed", true);
    }
}

int CommSocket::recv(void* buffer, uint64_t bufferLen)
              throw(SocketException) {
    // call recv() from socket.h
    int bytes_recieved;
    if ((bytes_recieved = ::recv(sockDesc, (raw_type *) buffer, bufferLen, 0)) < 0) {
        throw SocketException("CommSocket: recv failed", true);
    }

    return bytes_recieved;
}

int64_t CommSocket::recvFully(void *buffer, uint64_t bufferLen)
  throw(SocketException) {
  int64_t rcount = 0;
  int64_t len = ::recv(sockDesc, (raw_type *) buffer, bufferLen, 0);
  while (len > 0 && rcount + len < bufferLen) {
    rcount += len;
    len = ::recv(sockDesc, (raw_type *) (((char *) buffer) + rcount),
                 bufferLen - rcount, 0);
  }

  if (len < 0)
    throw SocketException("Receive failed (recv())");

  return rcount + len;
}


std::string CommSocket::getForeignAddress()
                      throw(SocketException) {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getpeername(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
        throw SocketException("CommSocket::getForeignAddress failed", true);
    }

    return inet_ntoa(addr.sin_addr);
}

unsigned short CommSocket::getForeignPort()
                         throw(SocketException) {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    if (getpeername(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
        throw SocketException("CommSocket::getForeignPort failed", true);
    }

    return ntohs(addr.sin_port);
}

void CommSocket::setRecvTimeout(unsigned int usec) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = usec;

    setsockopt(sockDesc, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
}
