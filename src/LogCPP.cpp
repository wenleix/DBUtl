#include "DBUtl/LogCPP.h"
#include "DBUtl/Log.h"

#include <iostream>
#include <cstring>

using namespace DBUtl;
using namespace DBUtl::Log;

namespace /* anon */ {
    struct SeverityMap { Severity sev; const char * str; } sevNames [] = {
        { ZERO, "ZERO" },
        { LOWEST, "LOWEST" },
        { DEBUG, "DEBUG" },
        { DEBUG0, "DEBUG0" },
        { DEBUG1, "DEBUG1" },
        { DEBUG2, "DEBUG2" },
        { DEBUG3, "DEBUG3" },
        { DEBUG4, "DEBUG4" },
        { TRACE, "TRACE" },
        { INFO, "INFO" },
        { WARNING, "WARNING" },
        { ERR, "ERR" },
        { CRIT, "CRIT" },
        { ALERT, "ALERT" },
        { EMERG, "EMERG" },
        { HIGHEST, "HIGHEST" },
        { DISABLED, "DISABLED" },
    };
}

namespace DBUtl {
    namespace Log {
        
        std::ostream* os = &std::cout;
        
        Severity stdToLevel(const char * str) {
            for( int i = 0; i < (sizeof sevNames)/(sizeof sevNames[0]); i++ ) {
                if( strcmp(sevNames[i].str, str) == 0 ) return sevNames[i].sev;
            }
            return DISABLED;
        }
        const char * levelToStd(Severity severity) {
            for( int i = 0; i < (sizeof sevNames)/(sizeof sevNames[0]); i++ ) {
                if( sevNames[i].sev == severity ) return sevNames[i].str;
            }
            return "DISABLED";
        }
    }
}

