
#include "Dispatcher.h"

#include <exception>

#include "DataStore.h"
#include "RequestBuilder.h"
#include "URLConnection.h"
#include "ReachabilityChecker.h"
#include "TrackerImpl.h"
#include "GAI.h"
#include "ReachabilityDelegate.h"

namespace GAI
{
	
	Dispatcher::Dispatcher( DataStore* data_store, bool opt_out, double dispatch_interval ) :
	mbOptOut( opt_out ),
	mDispatchInterval( dispatch_interval )
	{
		mDataStore = data_store;
	}
    
    bool Dispatcher::sendHit( Hit* hit )
    {
        // really: add this to the datastore
        throw "Not yet implemented";
    }
	
	void Dispatcher::queueDispatch()
    /// really: send outstanding hits if possible
	{
		throw "Not yet implemented";
	}
	
	void Dispatcher::cancelDispatch()
	{
		throw "Not yet implemented";
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