#ifndef __DBUTL_SOCKET_EXCEPTION__
#define __DBUTL_SOCKET_EXCEPTION__

#include <exception>
#include <string>

/**
 * The code sample is borrowed from
 * the book "TCP/IP" Sockets in C,
 * by Michael J. Donahoo and Kenneth L. Calvert.
 */

namespace DBUtl {

class SocketException : public std::exception {
public:
    /**
     * Construct a SocketException.
     * @param message message describing the exception
     * @param sysMsg true if system message (from strerror(errno))
     * should be postfixed to the user provided message.
     */
    SocketException(const std::string& message, bool sysMsg = false) throw();

    /**
     * Provided just to guarantee that no exception are thrown.
     */
    ~SocketException() throw();

    /**
     * Get the exception message
     * @return exception message
     */
    const char* getMessage() const throw();

private:
    std::string userMessage; // Exception message
};

} // namespace DBUtl

#endif /* __DBUTL_SOCKET_EXCEPTION__ */
