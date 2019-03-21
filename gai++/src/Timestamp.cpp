#include <string>
#include <sstream>
#include <event2/util.h>

#include "Timestamp.h"
#include "DataStore.h"

namespace GAI
{
    uint64_t Timestamp::generateTimestamp()
    ///
    /// Retrieves the current timestamp in milliseconds
    ///
    {
        uint64_t ms = 0;
        struct timeval tv;
        if(evutil_gettimeofday(&tv, NULL) == 0)
        {
            ms = tv.tv_sec * 1000.0 + tv.tv_usec * 0.001;
        }
        return ms;
    }
}
