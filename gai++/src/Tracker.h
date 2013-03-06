#include <exception>
#include <string>
#include <map>


#ifndef __Tracker_h__
#define __Tracker_h__


namespace GAI
{
	class GAI;
	class Transaction;
    
	class Tracker
	{
    public:
		 virtual bool sendView(std::string& aScreen) = 0;

		 virtual bool sendEvent(std::string& aCategory, std::string& aAction, std::string& aLabel, std::string& aValue) = 0;

		 virtual bool sendTransaction(Transaction* aTransaction) = 0;

		 virtual bool sendException(bool aIsFatal, std::string& aDescription) = 0;

		 virtual bool sendTimingWithCategory(std::string& aCategory, double aTime, std::string& aName, std::string& aLabel) = 0;

		 virtual  bool sendSocial(std::string& aNetwork, std::string& aAction, std::string& aTarget) = 0;

		 virtual bool setParameter(std::string& aName, std::string& aValue) = 0;

		 virtual std::string getParameter(std::string& aName) = 0;

		 virtual bool sendParameters(std::string& aTrackType, std::map<std::string, std::string>& aParameters) = 0;

		 virtual void close() = 0;

		 std::string getTrackingId();

		 void setAppName(std::string aAppName);

		 std::string getAppName();

		 void setAppId(std::string aAppId);

		 std::string getAppId();

		 void setAppVersion(std::string aAppVersion);

		 std::string getAppVersion();

		 void setAnonymize(bool aAnonymize);

		 bool isAnonymize();

		 void setUseHttps(bool aUseHttps);

		 bool isUseHttps();

		 void setSampleRate(double aSampleRate);

		 double getSampleRate();

		 std::string getClientId();

		 void setReferrerUrl(std::string aReferrerUrl);

		 std::string getReferrerUrl();

		 void setCampaignUrl(std::string aCampaignUrl);

		 std::string getCampaignUrl();

		 void setSessionStart(bool aSessionStart);

		 bool isSessionStart();

		 void setSessionTimeout(double aSessionTimeout);

		 double getSessionTimeout();
    protected:
        Tracker();
        
		std::string _trackingId;
		std::string _appName;
		std::string _appId;
		std::string _appVersion;
		bool _anonymize;
		bool _useHttps;
		double _sampleRate;
		std::string _clientId;
		std::string _referrerUrl;
		std::string _campaignUrl;
		bool _sessionStart;
		double _sessionTimeout;
        
	};
}

#endif
