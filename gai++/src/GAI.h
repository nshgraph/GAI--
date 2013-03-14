
#ifndef __GAI_h__
#define __GAI_h__

#include <string>
#include <map>

namespace GAI
{
	class Tracker;
	class Dispatcher;
    
	class GAI
	{
    public:
		static GAI* sharedInstance();
		
		Tracker* createTracker( const std::string& tracking_id );
		void removeTracker( const std::string& tracker_id );
		
		Tracker* getDefaultTracker() const;
		void setDefaultTracker( Tracker* tracker );
		
		std::string getProductString() const;
		void setProductString( const std::string& product_string );
		
		std::string getVersion() const;
		void setVersion( const std::string& version );
		
		bool isDebug() const;
		void setDebug( const bool debug );
		
		bool isOptOut() const;
		void setOptOut( const bool opt_out );
		
		double getDispatchInterval() const;
		void setDispatchInterval( const int dispatch_interval );
		
		void dispatch();
		
    private:
        GAI();
		
		typedef std::map<std::string, Tracker*> TrackerMap;
		TrackerMap mTrackers;
		
		Tracker* mDefaultTracker;
		
        std::string mProductString;
		std::string mVersion;
		
		bool mbDebug;	///< print information about data being sent
		
		Dispatcher* mDispatcher;
		
	};
}

#endif
