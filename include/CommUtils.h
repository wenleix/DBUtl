#ifndef __DBUTL_COMM_UTILS_H__
#define __DBUTL_COMM_UTILS_H__

#include <netinet/in.h>     // For sockaddr_in
#include <string>

#include "SocketException.h"

/**
 * The code sample is borrowed from
 * the book "TCP/IP" Sockets in C,
 * by Michael J. Donahoo and Kenneth L. Calvert.
 */

namespace DBUtl {

typedef void raw_type;

class Utils {
public:
	// Function to fill in address structure give an address and port.
	static void fillAddr(const std::string& address, unsigned short port,
			sockaddr_in &addr);
};

} // namespace DBUtl

#endif /* __DBUTL_COMM_UTILS_H__ */
