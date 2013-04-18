
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
    
	Analytics* Analytics::getInstance()
    ///
    /// Retrieve the singleton analytics instance
    ///
	{
		static Analytics* sharedInstance = NULL;
        if( sharedInstance == NULL )
		{
            sharedInstance = new Analytics();
		}
		
        return sharedInstance;
	}
	
	bool Analytics::init( const char* product_name, const char* product_version, const char* data_store_path )
    ///
    /// Init analytics instance
    ///
    /// @param product_name
    ///     Product name this analytics instance is intended to track
	///		
    /// @param data_store_path
    ///     Location to store cached hits
    ///
    {
		mProductName = product_name;
		mProductVersion = product_version;
		
        mDataStore = new DataStoreSqlite( data_store_path + mProductName );
		mDispatcher = new Dispatcher( *mDataStore, kOptOut, kDispatchInterval );
		
		mDefaultTracker = NULL;
		mbDebug = false;
	}
	
	Analytics::Analytics() :
	mProductName(),
    mProductVersion(),
	mDataStore( NULL ),
	mDispatcher( NULL ),
    mDefaultTracker( NULL ),
	mbDebug( false )
	///
	/// Constructor
	///
	{
	}
    
    Analytics::~Analytics()
    ///
    /// Destructor
    ///
    {
        delete mDataStore;
    }
    
    Tracker* Analytics::createTracker( const char* tracker_id )
    ///
    /// Create or retrieve a tracker by name. If the tracker already exists it is returned, if not it is created and added to the list
    ///
    /// @param tracker_id
    ///     Name to associate with tracker
    ///
    /// @return
    ///     New or retrieved Tracker
    ///
	{
        // first attempt to retrive the tracker with the same id
        TrackerMap::const_iterator it = mTrackers.find( tracker_id );
        if( it != mTrackers.end() )
		{
            return it->second;
		}
		
        // create a new tracker
        std::string client_id = ClientID::generateClientID(*mDataStore);
        Tracker* new_tracker = new TrackerImpl( *mDispatcher, client_id, tracker_id, mProductName, mProductVersion );
        mTrackers[ tracker_id ] = new_tracker;
        
        return new_tracker;
    }
    
    void Analytics::removeTracker( const char* tracker_id )
    ///
    /// Remove a tracker by name
    ///
    /// @param tracker_id
    ///     Tracker to delete
    ///
	{
        TrackerMap::iterator it = mTrackers.find( tracker_id );
        if( it != mTrackers.end() )
		{
            mTrackers.erase( it );
            delete it->second;
            if( it->second == mDefaultTracker)
                mDefaultTracker = NULL;
		}
    }
    
    Tracker* Analytics::getDefaultTracker() const
    ///
    /// Retrieve the default, unnamed tracker. Will be that set by setDefaultTracker
    ///
    /// @return
    ///     Default Tracker
    ///
	{
        return mDefaultTracker;
    }
	
	bool Analytics::setDefaultTracker( Tracker* tracker )
    ///
    /// Set the tracker to be returned by getDefaultTracker. Allows for quick, unnamed access of a single tracker
    /// NB: If tracker is not found in the list of trackers maintained by this instance the call will fail
    ///
    /// @param tracker
    ///     Tracker to make default
    ///
    /// @return
    ///     Whether the tracker was successfully set as default
    ///
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
    
    const char* Analytics::getProductName() const
    ///
    /// Retrieve the 'name' of the app sending data
    ///
    /// @return
    ///     Application name
    ///
	{
        return mProductName.c_str();
    }
    
    void Analytics::setProductName( const char* product_name )
    ///
    /// Set the 'name' of the app sending data. Will be used with any tracker created after this call
    ///
    /// @param product_name
    ///     Application name
    ///
    {
		mProductName = product_name;
    }
    
    const char* Analytics::getVersion() const
    ///
    /// Retrieve the 'version' of the app sending data
    ///
    /// @return
    ///     Application version
    ///
	{
        return mProductVersion.c_str();
    }
    
    bool Analytics::isDebug() const
    ///
    /// Retrieve whether debug data will be printed to the console
    ///
    /// @return
    ///     Whether debug data eill be printed to the console
    ///
	{
        return mbDebug;
    }
    
    void Analytics::setDebug( bool debug )
    ///
    /// Set whether debug data should be printed to the console
    ///
    /// @param debug
    ///     Whether debug data should be printed to the console
    ///
	{
		mbDebug = debug;
    }
    
    bool Analytics::isOptOut() const
    ///
    /// Retrieves whether the user has 'opted out' of data collection
    ///
    /// @return
    ///     Whether the user has decided to opt-out of data collection
    ///
	{
        return mDispatcher->isOptOut();
    }
    
    void Analytics::setOptOut( const bool opt_out )
    ///
    /// Set whether data should be collected at all
    ///
    /// @param opt_out
    ///     Whether the user has decided to opt-out of data collection
    ///
	{
        mDispatcher->setOptOut( opt_out );
    }
    void Analytics::setUseHttps(const bool aUseHttps)
    ///
    /// Set whether HTTPS will be used
    ///
    /// @param aUseHttps
    ///     Whether to use Https
    ///
    {
        mDispatcher->setUseHttps( aUseHttps );
    }
    
    bool Analytics::isUseHttps()
    ///
    /// Retreive whether the dispatcher will use secure connection
    ///
    /// @return
    ///     Whether HTTPS will be used
    ///
    {
        return mDispatcher->isUseHttps();
    }
    
    double Analytics::getDispatchInterval() const
    ///
    /// Retreive the interval between scheduled dispatches (in seconds)
    ///
    /// @return
    ///     Interval between dispatches
    ///
	{
        return mDispatcher->getDispatchInterval();
    }
    
    void Analytics::setDispatchInterval( const double dispatch_interval )
    ///
    /// Set the interval between scheduled dispatches (in seconds)
    ///
    /// @param dispatch_interval
    ///     Interval between dispatches
    ///
	{
        mDispatcher->setDispatchInterval( dispatch_interval );
    }
    
    void Analytics::dispatch()
    ///
    /// Queue an immediate dispatch (is asynchronous)
    ///
	{
        mDispatcher->queueDispatch();
    }
    
}

