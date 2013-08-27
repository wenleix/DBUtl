// TODO: use namespace

/**
 * The code sample is borrowed from
 * the book "TCP/IP" Sockets in C,
 * by Michael J. Donahoo and Kenneth L. Calvert.
 */
#ifndef __DBUTL_SOCKET_H__
#define __DBUTL_SOCKET_H__

#include <string>

#include "SocketException.h"

namespace DBUtl {

class Socket {
public:
	/**
	 * Close and deallocate this socket.
	 */
	~Socket();

	/**
	 * Get the local address.
	 * @return local address of the socket.
	 * @exception SocketException thrown if fetch fails.
	 */
	std::string getLocalAddress() throw (SocketException);

	/**
	 * Get the local port.
	 * @return local port of the socket.
	 * @exception SocketException thrown if fetch fails.
	 */
	unsigned short getLocalPort() throw (SocketException);

	/**
	 * Set the local port to the specified port and the local address
	 * to any interface.
	 * @param localPort local port
	 * @exception SocketException thrown if fetch fails.
	 */
	void setLocalPort(unsigned short localPort) throw (SocketException);

	/**
	 * Set the local port to the specified port and the local address
	 * to the specified address. If local port is not specified, a random
	 * port will be selected.
	 * @param localAddress local address
	 * @param localPort local port.
	 * @exception SocketException thrown if fetch fails.
	 */
	void setLocalAddressAndPort(const std::string& localAddress,
			unsigned short localPort = 0) throw (SocketException);

private:
	// Prevent the user from trying to use value semantics on this object
	Socket(const Socket& sock);
	void operator=(const Socket& sock);

protected:
	int sockDesc; // Socket descriptor
	Socket(int type, int protocol) throw (SocketException);
	Socket(int sockDesc);
};

} // namespace DBUtl

#endif /* __DBUTL_SOCKET_H__ */
