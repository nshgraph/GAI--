
#ifndef __Dispatcher_h__
#define __Dispatcher_h__

#include <string>

#include "HitStore.h"


namespace GAI
{
	class DataStore;
	class RequestBuilder;
	class URLConnection;
	class ReachabilityChecker;
	class TrackerImpl;
	class GAI;
    class Hit;
	class Dispatcher;
}

namespace GAI
{
	class Dispatcher : public HitStore
	{
    public:
		Dispatcher( DataStore& data_store, bool opt_out, double dispatch_interval );
        
        virtual bool storeHit( const Hit& hit );
		
		void queueDispatch();
		
		void cancelDispatch();
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
		
		int getDispatchInterval() const;
		void setDispatchInterval( const int dispatch_interval );
		
	private:
		DataStore& mDataStore;
		
		bool mbOptOut;			///< disable Google Analytics tracking
		int	mDispatchInterval;	///< dispatch interval in seconds
		
		uint64_t _cacheBuster;
		
	};
}

#endif
