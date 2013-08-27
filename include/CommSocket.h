#ifndef __DBUTL_COMMSOCKET_H__
#define __DBUTL_COMMSOCKET_H__

#include "Socket.h"
#include "SocketException.h"
#include "stdint.h"

#include <string> // TODO: XXX <---------------------------------------------------------------
/**
 * The code sample is borrowed from
 * the book "TCP/IP" Sockets in C,
 * by Michael J. Donahoo and Kenneth L. Calvert.
 */

namespace DBUtl {

/**
 * Communicating Socket
 */
class CommSocket: public Socket {
public:
	/**
	 * Establish a socket connection with the given address and port.
	 * @param foreignAddress ip address or name
	 * @param foreignPort port number
	 * @exception SocketException thrown if fetch fails.
	 */
	// TODO: change this to const char * <----------------------------------------------------------------------------
	void connect(const std::string& foreignAddress, unsigned short foreignPort)
			throw (SocketException);

	/**
	 * Write the given buffer to this socket.
	 * Call connect() before calling send().
	 * @param buffer buffer to be written.
	 * @param bufferLen number of bytes from buffer to be written.
	 * @exception SocketException thrown if fetch fails.
	 */
	void send(const void* buffer, uint64_t bufferLen) throw (SocketException);

	/**
	 * Read into the given buffer up to bufferLen bytes data from this socket.
	 * Call connect() before calling recv().
	 * @param buffer buffer to recieve the data.
	 * @param bufferLen maximum number of bytes to read into buffer.
	 * @return number of bytes read, 0 for EOF, and -1 for error.
	 * @exception SocketException thrown if fetch fails.
	 */
	int recv(void* buffer, uint64_t bufferLen) throw (SocketException);

	/**
	 *   Block until bufferLen bytes are read into the given buffer,
	 *   until the socket is closed or an error is encoutered.  The
	 *   socket must be connected before recvFully can be called.
	 *   @param buffer buffer to receive the data
	 *   @param bufferLen maximum number of bytes to read into buffer
	 *   @return number of bytes read, 0 for EOF, and -1 for error
	 *   @exception SocketException thrown if unable to receive data
	 */
	int64_t recvFully(void *buffer, uint64_t bufferLen) throw (SocketException);

	/**
	 * Get the foreign address.
	 */
	std::string getForeignAddress() throw (SocketException);

	/**
	 * Get the foreign port.
	 */
	unsigned short getForeignPort() throw (SocketException);

	/**
	 * Set the recv time out for a TCP connection.
	 * Parameter is micro-seconds.
	 */
	void setRecvTimeout(unsigned int usec);

protected:
	CommSocket(int type, int protocol) throw (SocketException);
	CommSocket(int newConnSD);

};

} // namespace DBUtl

#endif /* __DBUTL_COMMSOCKET_H__ */
