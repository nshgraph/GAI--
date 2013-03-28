
#ifndef __GAI_h__
#define __GAI_h__

#include <string>
#include <map>

namespace GAI
{
	class Tracker;
	class Dispatcher;
    class DataStore;
    
	class GAI
	{
    public:
		static GAI* sharedInstance( const std::string& product_name, const std::string& data_store_path );
		
		Tracker* createTracker( const std::string& tracking_id );
		void removeTracker( const std::string& tracker_id );
		
		Tracker* getDefaultTracker() const;
		void setDefaultTracker( Tracker* tracker );
		
		std::string getProductName() const;
		void setProductName( const std::string& product_name );
		
		std::string getVersion() const;
		void setVersion( const std::string& version );
		
		bool isDebug() const;
		void setDebug( const bool debug );
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
		
		double getDispatchInterval() const;
		void setDispatchInterval( const double dispatch_interval );
		
		void dispatch();
		
    private:
        GAI( const std::string& product_name, const std::string& data_store_path );
        ~GAI();
		
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
