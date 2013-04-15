
#ifndef __Timestamp_h__
#define __Timestamp_h__

namespace GAI
{
    class DataStore;
    
	class Timestamp
    ///
    /// This class handles the generation of timestamps
    ///
	{
    public:
        static unsigned long generateTimestamp();
	};
}

#endif
