
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
	class URLConnection;
	class TrackerImpl;
	class GAI;
    class Hit;
    
}

namespace GAI
{
	class Dispatcher : public HitStore
	{
    public:
		Dispatcher( DataStore& data_store, bool opt_out, double dispatch_interval );
		~Dispatcher();
        
        bool storeHit( const Hit& hit );
		
        void queueDispatch();
		
		void cancelDispatch();
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
        
        void setUseHttps(const bool aUseHttps);
        bool isUseHttps();
		
		int getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
	protected:	
		static void TimerThreadFunction( void* context );
		static void TimerCallback( evutil_socket_t file_descriptor, short events, void* context );
        
        virtual void dispatch();
		
		bool mbThreadRunning;
        bool mbCancelDispatch;
		event_base*	mDispatchEventBase;
		event*		mDispatchEvent;
        tthread::thread	mTimerThread;
		
		
        bool    mbUseHttps;         ///< whether to use a secure connection
		bool	mbOptOut;			///< disable Google Analytics tracking
		double	mDispatchInterval;	///< dispatch interval in seconds
        
		DataStore& mDataStore;
        URLConnection* mURLConnection;

		
	};
}

#endif
