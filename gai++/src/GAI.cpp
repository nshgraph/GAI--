
#include "gai++/GAI.h"
#include "gai++/Tracker.h"

#include "GAIDefines.h"
#include "TrackerImpl.h"
#include "Dispatcher.h"
#include "DataStoreSqlite.h"
#include "ClientID.h"
#include "Timestamp.h"
#include "DebugPrint.h"

// This is defined as extern in debugprint.h

namespace GAI
{
    bool sbDebugPrint = false;
    
    static std::auto_ptr<Analytics> sharedInstance;
    
#ifndef GAI_NO_DEPRECATED
	Analytics* Analytics::getInstance( const char* product_name, const char* product_version, const char* data_store_full_path, const char* client_id )
    ///
    /// Retrieve the singleton analytics instance
    ///
	{
		if( sharedInstance.get() == 0 )
		{
            if( product_name && product_version && data_store_full_path )
			{
				sharedInstance = std::auto_ptr<Analytics>(new Analytics( product_name, product_version, data_store_full_path, client_id ));
			}
		}
		
        return sharedInstance.get();
	}
    
    void Analytics::invalidateInstance()
    ///
    /// Invalidate the singleton instance of analytics (if it exists).
    ///
    {
        sharedInstance = std::auto_ptr<Analytics>(NULL);
    }
#endif
	
	Analytics::Analytics(  const char* product_name, const char* product_version, const char* data_store_full_path, const char* client_id ) :
	mProductName(product_name),
    mProductVersion(product_version),
	mDataStore( NULL ),
	mDispatcher( NULL ),
    mDefaultTracker( NULL )
	///
	/// Constructor
	///
	{
        mDataStore = new DataStoreSqlite( std::string(data_store_full_path) + ".gai" );
        mDataStore->open();
		mDispatcher = new Dispatcher( *mDataStore, kOptOut, kDispatchInterval );
        
        // handle the custom client_id
        if( client_id )
        {
            ClientID::setClientID( *mDataStore, client_id );
        }
	}
    
    Analytics::~Analytics()
    ///
    /// Destructor
    ///
    {
		for( TrackerMap::iterator it = mTrackers.begin(); it != mTrackers.end(); ++it )
		{
			delete it->second;
		}
		mTrackers.clear();
		
		delete mDispatcher;
        delete mDataStore;
    }
    
	///
	/// Starts the main Dispatcher event loop
	///
	/// @return Nothing
	///
	void Analytics::start()
	{
		mDispatcher->startEventLoop();
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
        TrackerMap::const_iterator it = mTrackers.find( std::string(tracker_id) );
        if( it != mTrackers.end() )
		{
            return it->second;
		}
		
        // create a new tracker
        std::string client_id = ClientID::generateClientID(*mDataStore);
        Tracker* new_tracker = new TrackerImpl( *mDispatcher, client_id.c_str(), tracker_id, mProductName.c_str(), mProductVersion.c_str() );
        mTrackers[ std::string(tracker_id) ] = new_tracker;
        
        if( mDefaultTracker == NULL )
            mDefaultTracker = new_tracker;
        
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
        TrackerMap::iterator it = mTrackers.find( std::string(tracker_id) );
        if( it != mTrackers.end() )
		{
			Tracker* tracker = it->second;
            mTrackers.erase( it );
            delete tracker;
            if( tracker == mDefaultTracker)
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
    
    const char* Analytics::getProductVersion() const
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
        return sbDebugPrint;
    }
    
    void Analytics::setDebug( bool debug )
    ///
    /// Set whether debug data should be printed to the console
    ///
    /// @param debug
    ///     Whether debug data should be printed to the console
    ///
	{
		sbDebugPrint = debug;
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
    
    bool Analytics::isUseHttps() const
    ///
    /// Retreive whether the dispatcher will use secure connection
    ///
    /// @return
    ///     Whether HTTPS will be used
    ///
    {
        return mDispatcher->isUseHttps();
    }
	
    void Analytics::setUseHttps( const bool use_https )
    ///
    /// Set whether HTTPS will be used
    ///
    /// @param aUseHttps
    ///     Whether to use Https
    ///
    {
        mDispatcher->setUseHttps( use_https );
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

