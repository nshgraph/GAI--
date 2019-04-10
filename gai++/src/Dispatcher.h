
#ifndef __Dispatcher_h__
#define __Dispatcher_h__

#include <string>
#include <event2/event.h>
#include <event2/util.h>
#include "tinythread.h"

#include "Hit.h"
#include "HitStore.h"

namespace GAI
{
	class DataStore;
	class URLConnection;
}

namespace GAI
{
	class Dispatcher : public HitStore
	{
    public:
		Dispatcher( DataStore& data_store,
				    const bool opt_out,
				    const double dispatch_interval,
				    const std::string& address,
				    const int port );
		~Dispatcher();
        
		void startEventLoop();
        
        bool storeHit( const Hit& hit );
		
        void queueDispatch();
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );

		void setAddress( const std::string& address, const int port );

		int getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
    protected:
		static void TimerThreadFunction( void* context );
		static void TimerCallback( evutil_socket_t file_descriptor, short events, void* context );
		static void RequestCallback( bool success, void* param );
        
        virtual void dispatch();
		
		bool mbThreadRunning;
		bool mbEvenLoopStarted;
        bool mbCancelDispatch;
		bool mbImmediateDispatch;
		uint64_t mPreviousTimestamp;

		event_base*	mDispatchEventBase;
		event*		mDispatchEvent;
        tthread::thread	mTimerThread;

		bool	mbOptOut;			///< disable Google Analytics tracking
		double	mDispatchInterval;	///< dispatch interval in seconds
        
		DataStore& mDataStore;
        URLConnection* mURLConnection;

		
	};
}

#endif
