#include "DBUtl/CommUtils.h"

#include <netdb.h>          // For hostent
#include <string.h>         // For memset

using namespace DBUtl;

void Utils::fillAddr(const std::string& address, unsigned short port,
                     sockaddr_in &addr) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    hostent *host;  // Resolve host
    if ((host = gethostbyname(address.c_str())) == NULL) {
        throw SocketException("Resolve name failed with gethostbyname()");
    }

    addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);

    addr.sin_port = htons(port);     // Assign port in network byte order
}

