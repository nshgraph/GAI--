
#ifndef __Tracker_h__
#define __Tracker_h__

#include <string>

namespace GAI
{
	class Transaction;
    
	class Tracker
	{
    public:
		 virtual bool sendView(const std::string& aScreen) = 0;

		 virtual bool sendEvent(const std::string& aCategory, const std::string& aAction, const std::string& aLabel, const std::string& aValue) = 0;

		 virtual bool sendTransaction(const Transaction* aTransaction) = 0;

		 virtual bool sendException(const bool aIsFatal, const std::string& aDescription) = 0;

		 virtual bool sendTimingWithCategory(const std::string& aCategory, const double aTime, const std::string& aName, const std::string& aLabel) = 0;

		 virtual  bool sendSocial(const std::string& aNetwork, const std::string& aAction, const std::string& aTarget) = 0;

		 virtual void close() = 0;

		 virtual std::string getTrackingId() const = 0;

		 virtual void setAppName(const std::string& aAppName) = 0;

		 virtual std::string getAppName() const = 0;

		 virtual void setAppId(const std::string& aAppId) = 0;

		 virtual std::string getAppId() const = 0;

		 virtual void setAppVersion(const std::string& aAppVersion) = 0;

		 virtual std::string getAppVersion() const = 0;

		 virtual void setAnonymize(const bool aAnonymize) = 0;

		 virtual bool isAnonymize() = 0;

		 virtual void setUseHttps(const bool aUseHttps) = 0;

		 virtual bool isUseHttps() = 0;

		 virtual void setSampleRate(const double aSampleRate) = 0;

		 virtual double getSampleRate() = 0;

		 virtual std::string getClientId() const = 0;

		 virtual void setReferrerUrl(const std::string& aReferrerUrl) = 0;

		 virtual std::string getReferrerUrl() = 0;

		 virtual void setCampaignUrl(const std::string& aCampaignUrl) = 0;

		 virtual std::string getCampaignUrl() = 0;

		 virtual void setSessionTimeout(const double aSessionTimeout) = 0;

		 virtual double getSessionTimeout() = 0;
        
	};
}

#endif
