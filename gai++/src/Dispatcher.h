#include <exception>
#include <string>


#ifndef __Dispatcher_h__
#define __Dispatcher_h__

#include "DataStore.h"
#include "RequestBuilder.h"
#include "URLConnection.h"
#include "ReachabilityChecker.h"
// #include "TrackerImpl.h"
// #include "GAI.h"
#include "ReachabilityDelegate.h"

namespace GAI
{
	class DataStore;
	class RequestBuilder;
	class URLConnection;
	class ReachabilityChecker;
	class TrackerImpl;
	class GAI;
    class Hit;
	// class ReachabilityDelegate;
	class Dispatcher;
}

namespace GAI
{
	class Dispatcher: public ReachabilityDelegate
	{
    public:
		Dispatcher( DataStore* aDataStore, std::string& aClientId, bool aOptOut, double aDispatchInterval );
		
		void reachabilityStatusChanged( ReachabilityChecker aReachability, int aStatus );
        
        bool sendHit( Hit* hit );
		
		void queueDispatch();
		
		void cancelDispatch();
		
		std::string getClientID();
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
		
		int getDispatchInterval() const;
		void setDispatchInterval( const int dispatch_interval );
		
	private:
		std::string mClientID;
		bool		mbOptOut;	///< disable Google Analytics tracking
		int			mDispatchInterval;	///< dispatch interval in seconds
		
		
		
		DataStore* _dataStore;
		RequestBuilder* _requestBuilder;
		URLConnection* _pendingDispatch;
		ReachabilityChecker* _reachabiltiy;
		uint64_t _cacheBuster;
		
	};
}

#endif
