
#include "GAI.h"
#include "Tracker.h"
#include "TrackerImpl.h"
#include "Dispatcher.h"

namespace GAI
{
    
    GAI* GAI::sharedInstance()
	{
        static GAI* sharedInstance = NULL;
        if( sharedInstance == NULL )
		{
            sharedInstance = new GAI();
		}
		
        return sharedInstance;
    }
    
    GAI::GAI()
    {
        mbDebug = false;
		mbOptOut = false;
		mDispatchInterval = 1800; // default dispatch interval is 1800 seconds (30 minutes)
    }
    
    Tracker* GAI::createTracker( const std::string& tracking_id )
	{
        // first attempt to retrive the tracker with the same id
        TrackerMap::const_iterator it = mTrackers.find( tracking_id );
        if( it != mTrackers.end() )
		{
            return it->second;
		}
		
        // create a new tracker
        Tracker* new_tracker = new TrackerImpl( mDispatcher, tracking_id, mProductString, mVersion );
        mTrackers[ tracking_id ] = new_tracker;
        
        return new_tracker;
    }
    
    void GAI::removeTracker( const std::string& tracker_id )
	{
        TrackerMap::iterator it = mTrackers.find( tracker_id );
        if( it != mTrackers.end() )
		{
            mTrackers.erase( it );
		}
    }
    
    Tracker* GAI::getDefaultTracker() const
	{
        return mDefaultTracker;
    }
    
    std::string GAI::getProductString() const
	{
        return mProductString;
    }
    
    void GAI::setProductString( const std::string& product_string )
    {
		mProductString = product_string;
    }
    
    std::string GAI::getVersion() const
	{
        return mVersion;
    }
    
    void GAI::setVersion( const std::string& version )
    {
        mVersion = version;
    }
    
    bool GAI::isDebug() const
	{
        return mbDebug;
    }
    
    void GAI::setDebug( bool debug )
	{
		mbDebug = debug;
    }
    
    bool GAI::isOptOut() const
	{
        return mbOptOut;
    }
    
    void GAI::setOptOut( const bool opt_out )
	{
        mbOptOut = opt_out;
    }
    
    void GAI::setDispatchInterval( const int dispatch_interval )
	{
        mDispatchInterval = dispatch_interval;
    }
    
    double GAI::getDispatchInterval() const
	{
        return mDispatchInterval;
    }
    
    void GAI::dispatch()
	{
        mDispatcher->queueDispatch();
    }
    
}

