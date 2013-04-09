
#include <event2/thread.h>

#include "GAI.h"
#include "GAIDefines.h"
#include "Tracker.h"
#include "TrackerImpl.h"
#include "Dispatcher.h"
#include "DataStoreSqlite.h"
#include "ClientID.h"
#include "Timestamp.h"

namespace GAI
{
    
    Analytics* Analytics::sharedInstance( const std::string& product_name, const std::string& data_store_path )
	{
        static Analytics* sharedInstance = NULL;
        if( sharedInstance == NULL )
		{
            sharedInstance = new Analytics( product_name, data_store_path );
		}
		
        return sharedInstance;
    }
    
    Analytics::Analytics( const std::string& product_name, const std::string& data_store_path ) :
	mProductName( product_name ),
    mVersion(""),
    mDefaultTracker(NULL),
	mbDebug( false )
    {
        mDataStore = new DataStoreSqlite( data_store_path + mProductName );
		mDispatcher = new Dispatcher( *mDataStore, kOptOut, kDispatchInterval );
        
        Timestamp::initializeTimestamp( *mDataStore );
	}
    
    Analytics::~Analytics()
    {
        delete mDataStore;
    }
    
    Tracker* Analytics::createTracker( const std::string& tracking_id )
	{
        // first attempt to retrive the tracker with the same id
        TrackerMap::const_iterator it = mTrackers.find( tracking_id );
        if( it != mTrackers.end() )
		{
            return it->second;
		}
		
        // create a new tracker
        std::string client_id = ClientID::generateClientID(*mDataStore);
        Tracker* new_tracker = new TrackerImpl( *mDispatcher, client_id, tracking_id, mProductName, mVersion );
        mTrackers[ tracking_id ] = new_tracker;
        
        return new_tracker;
    }
    
    void Analytics::removeTracker( const std::string& tracker_id )
	{
        TrackerMap::iterator it = mTrackers.find( tracker_id );
        if( it != mTrackers.end() )
		{
            mTrackers.erase( it );
		}
    }
    
    Tracker* Analytics::getDefaultTracker() const
	{
        return mDefaultTracker;
    }
	
	bool Analytics::setDefaultTracker( Tracker* tracker )
	{
        // we should check that this really is a tracker
        bool found_tracker = false;
        for( TrackerMap::const_iterator it = mTrackers.begin(), it_end = mTrackers.end();it != it_end && !found_tracker ; it++)
        {
            if( tracker == it->second )
                found_tracker = true;
        }
        
        if( found_tracker )
            mDefaultTracker = tracker;
        
        return found_tracker;
	}
    
    std::string Analytics::getProductName() const
	{
        return mProductName;
    }
    
    void Analytics::setProductName( const std::string& product_name )
    {
		mProductName = product_name;
    }
    
    std::string Analytics::getVersion() const
	{
        return mVersion;
    }
    
    void Analytics::setVersion( const std::string& version )
    {
        mVersion = version;
    }
    
    bool Analytics::isDebug() const
	{
        return mbDebug;
    }
    
    void Analytics::setDebug( bool debug )
	{
		mbDebug = debug;
    }
    
    bool Analytics::isOptOut() const
	{
        return mDispatcher->isOptOut();
    }
    
    void Analytics::setOptOut( const bool opt_out )
	{
        mDispatcher->setOptOut( opt_out );
    }
    
    double Analytics::getDispatchInterval() const
	{
        return mDispatcher->getDispatchInterval();
    }
    
    void Analytics::setDispatchInterval( const double dispatch_interval )
	{
        mDispatcher->setDispatchInterval( dispatch_interval );
    }
    
    void Analytics::dispatch()
	{
        mDispatcher->queueDispatch();
    }
    
}

