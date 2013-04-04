#include <string>
#include "tinythread.h"

#ifndef __ClientID_h__
#define __ClientID_h__

namespace GAI
{
    class DataStore;
    
	class ClientID
    ///
    /// This class handles the generation of client ids (UUIDs)
    ///
	{
    public:
		static std::string generateClientID(DataStore& datastore);
    private:
        static tthread::mutex clientIDMutex;
	};
}

#endif
