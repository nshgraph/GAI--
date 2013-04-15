#include <string>
#include <sstream>
#include <event2/util.h>

#include "Timestamp.h"
#include "DataStore.h"



namespace GAI
{
    
    unsigned long Timestamp::generateTimestamp( )
    ///
    /// This function retrieves a timestamp
    ///
    /// @return
    ///     The timestamp generated
    ///
    {
        unsigned long ms;
        struct timeval tv;
        if( evutil_gettimeofday(&tv, NULL) == 0)
        {
            ms = (tv.tv_sec) * 1000.0 + tv.tv_usec * 0.001;
        }
        return ms;
    }
    
}