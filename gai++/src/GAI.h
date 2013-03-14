
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
		void setDebug( const bool aDebug );
		
		bool isOptOut() const;
		void setOptOut( const bool aOptOut );
		
		double getDispatchInterval() const;
		void setDispatchInterval( const int aDispatchInterval );
		
		void dispatch();
		
    private:
        GAI();
		
		typedef std::map<std::string, Tracker*> TrackerMap;
		TrackerMap mTrackers;
		
		Tracker* mDefaultTracker;
		
        std::string mProductString;
		std::string mVersion;
		
		bool mbDebug;	///< print information about data being sent
		bool mbOptOut;	///< disable Google Analytics tracking
		
		Dispatcher* mDispatcher;
		int mDispatchInterval;		///< dispatch interval in seconds
		
	};
}

#endif
