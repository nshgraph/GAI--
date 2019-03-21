
#ifndef __GAI_h__
#define __GAI_h__

#include <map>
#include "Tracker.h"
#include "Transaction.h"
#include "TransactionItem.h"

namespace GAI
{
	
	class Dispatcher;
    class DataStore;
    
	class Analytics
	{
    public:
        
#ifndef GAI_NO_DEPRECATED
        ///
        /// Create or retrieve the singleton analytics instance. If an instance already exists then all parameters are optional.
        ///
        /// @param product_name
        ///     The application name to report analytics against
        /// @param product_version
        ///     The application version to report analytics against
        /// @param data_store_full_path
        ///     The full path of a file to store offline hits and other metadata. Must be writeable.
        /// @param client_id
        ///     Always optional. A pre-specified client_id for this instance
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		static Analytics* getInstance(  const char* product_name = NULL, const char* product_version = NULL, const char* data_store_full_path = NULL, const char* client_id = NULL );
        
        ///
        /// Invalidate the singleton instance of analytics (if it exists).
        ///
        static void invalidateInstance();
#endif

	    ///
	    /// Create an analytics instance
	    ///
	    /// @param product_name
	    ///     The application name to report analytics against
	    /// @param product_version
	    ///     The application version to report analytics against
	    /// @param data_store_full_path
	    ///     The full path of a file to store offline hits and other metadata. Must be writeable.
	    /// @param client_id
	    ///     Always optional. A pre-specified client_id for this instance
	    ///
	    Analytics( const char* product_name, const char* product_version, const char* data_store_full_path, const char* client_id = NULL );
		
		
        ///
        /// Destructor
        ///
        ~Analytics();
        
        
        ///
        /// Starts the main Dispatcher event loop
        ///
        /// @return Nothing
        ///
		void start();
        
        
        ///
        /// Create or retrieve a tracker by name. If the tracker already exists it is returned, if not it is created and added to the list
        ///
        /// @param tracker_id
        ///     Name to associate with tracker
        ///
        /// @return
        ///     New or retrieved Tracker
        ///
		Tracker* createTracker( const char* tracking_id );
        
        
        ///
        /// Remove a tracker by name
        ///
        /// @param tracker_id
        ///     Tracker to delete
        ///
		void removeTracker( const char* tracker_id );
        
        
        ///
        /// Retrieve the default, unnamed tracker. Will be that set by setDefaultTracker
        ///
        /// @return
        ///     Default Tracker
        ///
		Tracker* getDefaultTracker() const;
        
        
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
		bool setDefaultTracker( Tracker* tracker );
		
        
        ///
        /// Retrieve the 'name' of the app sending data
        ///
        /// @return
        ///     Application name
        ///
		const char* getProductName() const;
		
        
        ///
        /// Retrieve the 'version' of the app sending data
        ///
        /// @return
        ///     Application version
        ///
		const char* getProductVersion() const;
		
        
        ///
        /// Retrieve whether debug data will be printed to the console
        ///
        /// @return
        ///     Whether debug data eill be printed to the console
        ///
		bool isDebug() const;
        
        
        ///
        /// Set whether debug data should be printed to the console
        ///
        /// @param debug
        ///     Whether debug data should be printed to the console
        ///
		void setDebug( const bool debug );

        
        ///
        /// Retrieves whether the user has 'opted out' of data collection
        ///
        /// @return
        ///     Whether the user has decided to opt-out of data collection
        ///
		bool isOptOut() const;
        
        
        ///
        /// Set whether data should be collected at all
        ///
        /// @param opt_out
        ///     Whether the user has decided to opt-out of data collection
        ///
		void setOptOut( const bool opt_out );
        
        
        ///
        /// Retreive whether the dispatcher will use secure connection
        ///
        /// @return
        ///     Whether HTTPS will be used
        ///
        bool isUseHttps() const;
        
        
        ///
        /// Set whether HTTPS will be used
        ///
        /// @param aUseHttps
        ///     Whether to use Https
        ///
		void setUseHttps(const bool use_https );
		
        
        ///
        /// Retreive the interval between scheduled dispatches (in seconds)
        ///
        /// @return
        ///     Interval between dispatches
        ///
		double getDispatchInterval() const;
        
        
        ///
        /// Set the interval between scheduled dispatches (in seconds)
        ///
        /// @param dispatch_interval
        ///     Interval between dispatches
        ///
		void setDispatchInterval( const double dispatch_interval );
		
        
        ///
        /// Queue an immediate dispatch (is asynchronous)
        ///
		void dispatch();
		
    private:
		typedef std::map<std::string, Tracker*> TrackerMap;
		TrackerMap mTrackers;
		
		Tracker* mDefaultTracker;
		
        std::string mProductName;
		std::string mProductVersion;
		bool mbUseHttps;
		
		Dispatcher* mDispatcher;
        DataStore* mDataStore;
		
	};
}

#endif
