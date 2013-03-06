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
	// class ReachabilityDelegate;
	class Dispatcher;
}

namespace GAI
{
	class Dispatcher: public ReachabilityDelegate
	{
		 std::string _clientID;
		 bool _optOut;
		 double _dispatchInterval;
		 DataStore* _dataStore;
		 RequestBuilder* _requestBuilder;
		 URLConnection* _pendingDispatch;
		 ReachabilityChecker* _reachabiltiy;
		 uint64_t _cacheBuster;

		Dispatcher(DataStore* aDataStore, std::string& aClientId, bool aOptOut, double aDispatchInterval);

		void reachabilityStatusChanged(ReachabilityChecker aReachability, int aStatus);

		void queueDispatch();

		void cancelDispatch();

		std::string getClientID();

		void setOptOut(bool aOptOut);

		bool isOptOut();

		void setDispatchInterval(double aDispatchInterval);

		double getDispatchInterval();
	};
}

#endif
