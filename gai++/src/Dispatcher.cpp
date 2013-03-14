#include <exception>
#include <string>


#include "Dispatcher.h"
#include "DataStore.h"
#include "RequestBuilder.h"
#include "URLConnection.h"
#include "ReachabilityChecker.h"
#include "TrackerImpl.h"
#include "GAI.h"
#include "ReachabilityDelegate.h"

namespace GAI
{
	
	Dispatcher::Dispatcher( DataStore* aDataStore, std::string& aClientId, bool aOptOut, double aDispatchInterval )
	{
	}
	
	void Dispatcher::reachabilityStatusChanged( ReachabilityChecker aReachability, int aStatus )
	{
		throw "Not yet implemented";
	}
    
    bool Dispatcher::sendHit( Hit* hit )
    {
        // really: add this to the datastore
        throw "Not yet implemented";
    }
	
	void Dispatcher::queueDispatch()
	{
		throw "Not yet implemented";
	}
	
	void Dispatcher::cancelDispatch()
	{
		throw "Not yet implemented";
	}
	
	std::string Dispatcher::getClientID()
	{
		return mClientID;
	}
	
	bool Dispatcher::isOptOut() const
	{
		return mbOptOut;
	}
	
	void Dispatcher::setOptOut( const bool opt_out )
	{
		mbOptOut = opt_out;
	}
	
	int Dispatcher::getDispatchInterval() const
	{
		return mDispatchInterval;
	}
	
	void Dispatcher::setDispatchInterval( const int dispatch_interval )
	{
		mDispatchInterval = dispatch_interval;
	}
	
}