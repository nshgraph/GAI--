
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
    
    GAI* GAI::sharedInstance( const std::string& product_name, const std::string& data_store_path )
	{
        static GAI* sharedInstance = NULL;
        if( sharedInstance == NULL )
		{
            sharedInstance = new GAI( product_name, data_store_path );
		}
		
        return sharedInstance;
    }
    
    GAI::GAI( const std::string& product_name, const std::string& data_store_path ) :
	mProductName( product_name ),
	mbDebug( false )
    {
        mDataStore = new DataStoreSqlite( data_store_path + mProductName );
		mDispatcher = new Dispatcher( *mDataStore, kOptOut, kDispatchInterval );
        
        Timestamp::initializeTimestamp( mDataStore );
	}
    
    GAI::~GAI()
    {
        delete mDataStore;
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
        std::string client_id = ClientID::generateClientID(*mDataStore);
        Tracker* new_tracker = new TrackerImpl( *mDispatcher, client_id, tracking_id, mProductName, mVersion );
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
	
	void GAI::setDefaultTracker( Tracker* tracker )
	{
		mDefaultTracker = tracker;
	}
    
    std::string GAI::getProductName() const
	{
        return mProductName;
    }
    
    void GAI::setProductName( const std::string& product_name )
    {
		mProductName = product_name;
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
        return mDispatcher->isOptOut();
    }
    
    void GAI::setOptOut( const bool opt_out )
	{
        mDispatcher->setOptOut( opt_out );
    }
    
    double GAI::getDispatchInterval() const
	{
        return mDispatcher->getDispatchInterval();
    }
    
    void GAI::setDispatchInterval( const double dispatch_interval )
	{
        mDispatcher->setDispatchInterval( dispatch_interval );
    }
    
    void GAI::dispatch()
	{
        mDispatcher->queueDispatch();
    }
    
}

