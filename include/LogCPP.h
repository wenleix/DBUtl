#ifndef __LOG_CPP_H__
#define __LOG_CPP_H__

#include <iostream>
#include <cstring>

#include "DBUtl/Log.h"

// The following code is borrowed from the Indexing Project.
// owner: Ben Sowell

/**
 * Simple C++ logging class. You can write a log message as follows:
 *    LOG(LEVEL) << this is a message << std::endl
 * where level is of type Severity
 *
 * Logging can be disabled either statically (using the LOG_LIMIT macro) or
 * dynamically (using IndexLog::setLevel()). The log can be written to any
 * C++ ostream. By default it uses std::cout.
 *
 * Note: This class is based on an article at
 *       http://www.ddj.com/cpp/201804215
 */


namespace DBUtl {

#ifndef LOG_LIMIT
#define LOG_LIMIT DBUtl::Log::ZERO
#endif

#define LOG(level) \
    if (DBUtl::Log :: level < LOG_LIMIT) ; \
    else if (DBUtl::Log :: level < DBUtl::Log::getLevel()) ; \
    else DBUtl::Log::Get()

/**
 * Logging levels -- DBUtl/Log.h is authoritative

 enum Severity {
	ZERO = 0,
	LOWEST = 0,

	DEBUG = 2,

	TRACE = 11,
	INFO = 12,
	WARNING = 13,
	ERR = 14,
	CRIT = 15,
	ALERT = 16,
	EMERG = 17,

	HIGHEST = 127,
	DISABLED = 127

 };
 */


    namespace Log {
        
        /**
         * The output stream to use for logging. Defaults to cout.
         */
            extern std::ostream* os;

        /**
         * The current logging level. Only messages written at this level or
         * higher level will be logged.
         */
        //  extern Severity _severityLevel; -- shared with DBUtl/Log.h

        /**
         * Convert between string representation of a log level and its
         * Severity value.
         */
        Severity stdToLevel(const char* str);
        const char * levelToStd(Severity severity);

        /**
         * Return the current global logging level.
         */
        //  Severity getLevel() { return level; } -- in DBUtl/Log.h

        /**
         * Set the current global logging level. All messages written at
         * level or higher will be logged to the ostream os.
         */
        //  void setLevel(Severity myLevel) { level = myLevel; } -- in DBUtl/Log.h

        /**
         * Set the logging stream.
         */
        inline void setStream(std::ostream& myOS) {
            os = &myOS;
        }

        /**
         * Return the current logging stream. This should only be called
         * through the LOG macro.
         */
        inline std::ostream& Get() {
            (*os) << " - ";
            return (*os);
        }
    }

} // namespace DBUtl

#endif /*__LOG_CPP_H__ */
