

#ifndef __TrackerImpl_h__
#define __TrackerImpl_h__

#include <string>
#include <map>
#include "tinythread.h"

#include "HitType.h"
#include "Tracker.h"

namespace GAI
{
	class Transaction;
	class HitStore;
    class Model;
    
	class TrackerImpl: public Tracker
	{
    public:
		TrackerImpl(HitStore& aHitStore, const std::string& aClientID, const std::string& aTrackingID,
                    const std::string& aAppName, const std::string& aAppVersion);
        
        ~TrackerImpl();

		bool sendView(const std::string& aScreen);

		bool sendEvent(const std::string& aCategory, const std::string& aAction, const std::string& aLabel, const std::string& aValue);

		bool sendTransaction(const Transaction* aTransaction);

		bool sendException(const bool aIsFatal, const std::string& aDescription);

		bool sendTimingWithCategory(const std::string& aCategory, double aTime, const std::string& aName, const std::string& aLabel);

		bool sendSocial(const std::string& aNetwork, const std::string& aAction, const std::string& aTarget);

		void close();
        
        std::string getTrackingId() const;
        
        void setAppName(const std::string& aAppName);
        
        std::string getAppName() const;
        
        void setAppId(const std::string& aAppId);
        
        std::string getAppId() const;
        
        void setAppVersion(const std::string& aAppVersion);
        
        std::string getAppVersion() const;
        
        void setAnonymize(const bool aAnonymize);
        
        bool isAnonymize();
        
        void setSampleRate(const double aSampleRate);
        
        double getSampleRate();
        
        std::string getClientId() const;
        
        void setReferrerUrl(const std::string& aReferrerUrl);
        
        std::string getReferrerUrl();
        
        void setCampaignUrl(const std::string& aCampaignUrl);
        
        std::string getCampaignUrl();
        
        void setSessionTimeout(const double aSessionTimeout);
        
        double getSessionTimeout();

    private:
        typedef std::map<std::string, std::string> ParameterMap;
    private:
        void setTrackingId(const std::string& aTrackingId);
        void setClientId(const std::string& aClientId);
        bool internalSend(const HitType aType, const ParameterMap& aParameters);
        
        bool mbSessionStart; // Whether the first hit has been sent
        bool mbTrackerOpen; // Whether this tracker is 'open'. If not Hit's can't be sent
        
        Model* mModel; // The model used to store parameters for use with the hit
        tthread::mutex mModelLock; // Lock for ensuring single access to the mModel variable
        HitStore& mHitStore; // Reference to the Hit Store to use for sending hits
        double mSampleRate; // sample rate of hits
        std::string mHttpDispatchUrl; // URL to use for unsecured connections
        std::string mHttpsDispatchUrl; // URL to use for secured connections
        bool mbUseHttps; // Whether to use a secure connection
        
		double mSessionTimeout; // session timeout
        
	};
}

#endif
