

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
		TrackerImpl(HitStore& aHitStore, const char* aClientID, const char* aTrackingID,
                    const char* aAppName, const char* aAppVersion);
        
        ~TrackerImpl();

		bool sendView(const char* aScreen);

		bool sendEvent(const char* aCategory, const char* aAction, const char* aLabel, const char* aValue);

		bool sendTransaction(const Transaction* aTransaction);

		bool sendException(const bool aIsFatal, const char* aDescription);

		bool sendTimingWithCategory(const char* aCategory, double aTime, const char* aName, const char* aLabel);

		bool sendSocial(const char* aNetwork, const char* aAction, const char* aTarget);

		void close();
        
        const char* getTrackingId() const;
        
        void setAppName(const char* aAppName);
        
        const char* getAppName() const;
        
        void setAppId(const char* aAppId);
        
        const char* getAppId() const;
        
        void setAppVersion(const char* aAppVersion);
        
        const char* getAppVersion() const;
        
        void setAnonymize(const bool aAnonymize);
        
        bool isAnonymize();
        
        void setSampleRate(const double aSampleRate);
        
        double getSampleRate();
        
        const char* getClientId() const;
        
        void setReferrerUrl(const char* aReferrerUrl);
        
        const char* getReferrerUrl();
        
        void setCampaignUrl(const char* aCampaignUrl);
        
        const char* getCampaignUrl();
        
        void setSessionTimeout(const double aSessionTimeout);
        
        double getSessionTimeout();
        
        void setViewportSize( const char* aViewportSize );

    private:
        typedef std::map<std::string, std::string> ParameterMap;
    private:
        void setTrackingId(const char* aTrackingId);
        void setClientId(const char* aClientId);
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
