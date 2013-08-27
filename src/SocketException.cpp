#include "DBUtl/SocketException.h"

#include <cerrno>          // For errno
#include <cstring>         // For strerror

using namespace DBUtl;

// constructor
SocketException::SocketException(const std::string& message, bool sysMsg)
    throw() : userMessage(message) {
    if (sysMsg) {
        userMessage.append(": ");
        userMessage.append(strerror(errno));
    }
}

// destructor
SocketException::~SocketException() throw() {
}

const char* SocketException::getMessage() const throw() {
    return userMessage.c_str();
}

