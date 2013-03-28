
#ifndef __Dispatcher_h__
#define __Dispatcher_h__

#include <string>

#include "HitStore.h"

#include <event.h>
#include <evutil.h>

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
		~Dispatcher();
        
        virtual bool storeHit( const Hit& hit );
		
		virtual void queueDispatch();
		
		void cancelDispatch();
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
		
		int getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
	private:
		void createTimerThread();
		static void* timerThread( void* context );
		static void timerCallback( evutil_socket_t file_descriptor, short events, void* context );
		
		pthread_t	mTimerThread;
		
		event_base*	mDispatchEventBase;
		event*		mDispatchEvent;
		
		DataStore& mDataStore;
		
		bool	mbOptOut;			///< disable Google Analytics tracking
		double	mDispatchInterval;	///< dispatch interval in seconds
		
		uint64_t _cacheBuster;
		
	};
}

#endif
