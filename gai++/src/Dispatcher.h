
#ifndef __Dispatcher_h__
#define __Dispatcher_h__

#include <string>
#include <event.h>
#include <evutil.h>
#include "tinythread.h"

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
		~Dispatcher();
        
        virtual bool storeHit( const Hit& hit );
		
		virtual void queueDispatch();
		
		void cancelDispatch();
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
		
		int getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
	protected:
		DataStore& mDataStore;
	
	private:
		static void TimerThreadFunction( void* context );
		static void TimerCallback( evutil_socket_t file_descriptor, short events, void* context );
		
		bool mbThreadRunning;
		event_base*	mDispatchEventBase;
		event*		mDispatchEvent;
        tthread::thread	mTimerThread;
		
		
		bool	mbOptOut;			///< disable Google Analytics tracking
		double	mDispatchInterval;	///< dispatch interval in seconds
		
		uint64_t _cacheBuster;
		
	};
}

#endif
