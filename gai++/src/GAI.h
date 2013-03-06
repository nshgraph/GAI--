#include <exception>
#include <string>
#include <map>


#ifndef __GAI_h__
#define __GAI_h__

namespace GAI
{
	class Tracker;
	class Dispatcher;
	class GAI;
    
	class GAI
	{
    public:
		static GAI* sharedInstance();
    public:
		Tracker* createTracker(const std::string& aTrackingId);

		void dispatch();

		void removeTracker(const std::string& aTrackerId);

		std::string getProductString() const;

		std::string getVersion() const;
        
		void setProductString(const std::string& productString);
        
		void setVersion(const std::string& version);

		Tracker* getDefaultTracker() const;

		void setDebug(bool aDebug);

		bool isDebug() const;

		void setOptOut(const bool aOptOut);

		bool isOptOut() const;

		void setDispatchInterval(const double aDispatchInterval);

		double getDispatchInterval() const;
    
        typedef std::map<std::string, Tracker*> TrackerMap;
    
        GAI();
        
        std::string _productString;
		std::string _version;
		Tracker* _defaultTracker;
		bool _debug;
		bool _optOut;
		double _dispatchInterval;
		Dispatcher* _dispatcher;
		TrackerMap _trackers;
	};
}

#endif
