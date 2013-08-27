#ifndef __DBUTL_TCPSOCKET_H__
#define __DBUTL_TCPSOCKET_H__

#include "CommSocket.h"
#include "SocketException.h"

#include <string>
/**
 * The code sample is borrowed from
 * the book "TCP/IP" Sockets in C,
 * by Michael J. Donahoo and Kenneth L. Calvert.
 */

namespace DBUtl {


/**
 * TCP socket for communication with other TCP sockets.
 */
class TCPSocket : public CommSocket {
public:
    /**
     * Construct a TCP socket with no connection.
     * @exception SocketException thrown if unable to create TCP socket
     */
    TCPSocket() throw(SocketException);

    /**
     * Construct a TCP socket with a connection to the given foreign address
     * and port.
     * @param foreignAddress foreign address (IP address or name).
     * @param foreignPort foreign port.
     * @exception SocketException thrown if unable to create TCP socket
     */
    TCPSocket(const std::string &foreignAddress, unsigned short foreignPort)
        throw(SocketException);

private:
    // Access for TCPServerSocket::accept() connection creation.
    friend class TCPServerSocket;
    TCPSocket(int newConnSD);
};

/**
 * TCP socket class for servers.
 */
class TCPServerSocket : public Socket {
public:
    /**
     * Construct a TCP socket for use with a server, accepting connections
     * on the specified port on any interface.
     * @param localPort local port of server socket,
     *                   0: system assigns an unused port.
     * @param queueLen maximum queue length for outstanding connection
     *                 requests (default 5).
     * @exception  SocketException thrown if unable to create TCP server socket.
     */
    TCPServerSocket(unsigned short localPort, int queueLen = 5)
        throw(SocketException);

    /**
     * Construct a TCP socket for use with a server, accepting connections
     * on specified port on the interface specified by the given address.
     * @param localAddress local interface (address) of the server socket.
     * @param localPort local port of server socket.
     * @param queueLen maximum queue length for outstanding connection
     *                 requests (default 5).
     * @exception  SocketException thrown if unable to create TCP server socket.
     */
    TCPServerSocket(const std::string& localAddress, unsigned short localPort,
                    int queueLen = 5) throw(SocketException);

    /**
     * Blocks until a new connection is established on this server.
     * @return new connection socket.
     * @exception SocketException thrown if attempt to accespt a new connection fails.
     */
    TCPSocket* accept() throw(SocketException);

private:
    void setListen(int queueLen) throw(SocketException);
};

} // namespace DBUtl

#endif /* __DBUTL_TCPSOCKET_H__ */
