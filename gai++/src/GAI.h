
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
		static Analytics* getInstance(  const char* product_name = NULL, const char* product_version = NULL, const char* data_store_path = NULL );
        ~Analytics();
		
		Tracker* createTracker( const char* tracking_id );
		void removeTracker( const char* tracker_id );
		
		Tracker* getDefaultTracker() const;
		bool setDefaultTracker( Tracker* tracker );
		
		const char* getProductName() const;
		
		const char* getVersion() const;
		
		bool isDebug() const;
		void setDebug( const bool debug );
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
        
        bool isUseHttps() const;
		void setUseHttps(const bool use_https );
		
		double getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
		void dispatch();
		
    private:
        Analytics( const char* product_name, const char* product_version, const char* data_store_path );
		
		typedef std::map<std::string, Tracker*> TrackerMap;
		TrackerMap mTrackers;
		
		Tracker* mDefaultTracker;
		
        std::string mProductName;
		std::string mProductVersion;
		
		bool mbDebug;	///< print information about data being sent
		
		Dispatcher* mDispatcher;
        DataStore* mDataStore;
		
	};
}

#endif
