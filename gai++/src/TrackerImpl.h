#include <exception>
#include <string>
#include <map>


#ifndef __TrackerImpl_h__
#define __TrackerImpl_h__

#include "Dispatcher.h"
#include "TrackerState.h"
#include "HitType.h"
#include "Tracker.h"

namespace GAI
{
	class Dispatcher;
	class Transaction;
    
	class TrackerImpl: public Tracker
	{
    public:
		TrackerImpl(Dispatcher* aDispatcher, std::string& aTrackingID, std::string& aAppName, std::string& aAppVersion);

		bool sendView(std::string& aScreen);

		bool sendEvent(std::string& aCategory, std::string& aAction, std::string& aLabel, std::string& aValue);

		bool sendTransaction(Transaction* aTransaction);

		bool sendException(bool aIsFatal, std::string& aDescription);

		bool sendTimingWithCategory(std::string& aCategory, double aTime, std::string& aName, std::string& aLabel);

		bool sendSocial(std::string& aNetwork, std::string& aAction, std::string& aTarget);

		bool setParameter(std::string& aName, std::string& aValue);

		std::string getParameter(std::string& aName);

		bool sendParameters(std::string& aTrackType, std::map<std::string, std::string>& aParameters);

		void close();

		 
    private:
        bool internalSend(HitType aType, std::map<std::string, std::string> aParameters);
        
        Dispatcher* _dispatcher;
        TrackerState _trackerState;
        int64_t _trackCount;
        int64_t _hitCount;
        double _lastActiveTime;
        double _lastTrackTime;
	};
}

#endif
