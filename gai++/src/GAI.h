
#ifndef __GAI_h__
#define __GAI_h__

#include <string>
#include <map>

namespace GAI
{
	class Tracker;
	class Dispatcher;
    class DataStore;
    
	class Analytics
	{
    public:
		static Analytics* sharedInstance( const char* product_name, const char* data_store_path );
		
		Tracker* createTracker( const char* tracking_id );
		void removeTracker( const char* tracker_id );
		
		Tracker* getDefaultTracker() const;
		bool setDefaultTracker( Tracker* tracker );
		
		const char* getProductName() const;
		void setProductName( const char* product_name );
		
		const char* getVersion() const;
		void setVersion( const char* version );
		
		bool isDebug() const;
		void setDebug( const bool debug );
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
        
        void setUseHttps(const bool aUseHttps);
        bool isUseHttps();
		
		double getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
		void dispatch();
		
    private:
        Analytics( const char* product_name, const char* data_store_path );
        ~Analytics();
		
		typedef std::map<std::string, Tracker*> TrackerMap;
		TrackerMap mTrackers;
		
		Tracker* mDefaultTracker;
		
        std::string mProductName;
		std::string mVersion;
		
		bool mbDebug;	///< print information about data being sent
		
		Dispatcher* mDispatcher;
        DataStore* mDataStore;
		
	};
}

#endif
