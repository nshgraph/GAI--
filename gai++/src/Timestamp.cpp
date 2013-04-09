#include <string>
#include <sstream>
#include <event2/util.h>

#include "Timestamp.h"
#include "DataStore.h"



namespace GAI
{
    
    long Timestamp::BaseTime = 0;
    
    void Timestamp::initializeTimestamp(DataStore& store)
    {
        if( BaseTime == 0 )
        {
            std::string base_time = store.fetchProperty( "base_timestamp");
            // attempt to parse the stored string into a long
            std::stringstream base_time_stream_in(base_time);
            long retrieved_base_time;
            base_time_stream_in >> retrieved_base_time;
            
            if( base_time.empty() || retrieved_base_time == 0 )
            {
                // create a timestamp
                struct timeval tv;
                if( evutil_gettimeofday(&tv, NULL) == 0)
                    retrieved_base_time = tv.tv_sec;
                // convert into a string and store
                std::stringstream base_time_stream_out;
                base_time_stream_out << retrieved_base_time;
                store.addProperty("base_timestamp", base_time_stream_out.str());
            }
            BaseTime = retrieved_base_time;
            
        }
    }
    
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
            ms = (tv.tv_sec - BaseTime) * 1000.0 + tv.tv_usec * 0.001;
        }
        return ms;
    }
    
}