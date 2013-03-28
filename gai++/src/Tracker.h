
#ifndef __Tracker_h__
#define __Tracker_h__

#include <string>

namespace GAI
{
	class Transaction;
    
	class Tracker
	{
    public:
        
        ///
        /// Send a 'hit' representing the user viewing a screen
        ///
        /// @param aScreen
        ///     Name of the screen associated with this view
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendView(const std::string& aScreen) = 0;
        
        ///
        /// Send a 'hit' representing a user generated event
        ///
        /// @param aCategory
        ///     Category of this event
        /// @param aAction
        ///     Action of this event
        /// @param aLabel
        ///     Label of this event
        /// @param aValue
        ///     Value of this event
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendEvent(const std::string& aCategory, const std::string& aAction, const std::string& aLabel, const std::string& aValue) = 0;
        
        ///
        /// Send a 'hit' representing a Transaction
        ///
        /// @param aTransaction
        ///     Transaction to send
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendTransaction(const Transaction* aTransaction) = 0;
        
        ///
        /// Send a 'hit' representing an exception
        ///
        /// @param aIsFatal
        ///     Whether the exception was fatal
        /// @param aDescription
        ///     Description of exception thrown
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendException(const bool aIsFatal, const std::string& aDescription) = 0;
        
        ///
        /// Send a 'hit' representing a timing event
        ///
        /// @param aCategory
        ///     Category of the timed event
        /// @param aTime
        ///     Time associated with the event
        /// @param aName
        ///     Name for event
        /// @param aLabel
        ///     Label for event
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendTimingWithCategory(const std::string& aCategory, const double aTime, const std::string& aName, const std::string& aLabel) = 0;
        
        ///
        /// Send a 'hit' representing social media interaction
        ///
        /// @param aNetwork
        ///     Name of social media network
        /// @param aAction
        ///     Action for event
        /// @param aTarget
        ///     Target for event
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual  bool sendSocial(const std::string& aNetwork, const std::string& aAction, const std::string& aTarget) = 0;

        ///
        /// Close the tracker. Any further events will be ignored
        ///
		 virtual void close() = 0;
        
        ///
        /// Retreive the id of this tracker
        ///
        /// @return
        ///     Tracker ID
        ///
		 virtual std::string getTrackingId() const = 0;
        
        ///
        /// Set the App name reported by this tracker. Will only work before the first Hit is sent
        ///
        /// @param aAppName
        ///     App Name
        ///
		 virtual void setAppName(const std::string& aAppName) = 0;
        
        ///
        /// Retreive the app name this tracker is tracking
        ///
        /// @return
        ///     App Name
        ///
		 virtual std::string getAppName() const = 0;
        
        ///
        /// Set the App ID reported by this tracker. Will only work before the first Hit is sent
        ///
        /// @param aAppId
        ///     App ID
        ///
		 virtual void setAppId(const std::string& aAppId) = 0;
        
        ///
        /// Retreive the app id this tracker is tracking
        ///
        /// @return
        ///     App ID
        ///
		 virtual std::string getAppId() const = 0;
        
        ///
        /// Set the App version reported by this tracker. Will only work before the first Hit is sent
        ///
        /// @param aAppVersion
        ///     App Version
        ///
		 virtual void setAppVersion(const std::string& aAppVersion) = 0;
        
        ///
        /// Retreive the app version this tracker is tracking
        ///
        /// @return
        ///     App Version
        ///
		 virtual std::string getAppVersion() const = 0;
        
        ///
        /// Set whether the user's IP address will be anonymized. Will only work before the first Hit is sent
        ///
        /// @param aAnonymize
        ///    Seetting for anonymization
        ///
		 virtual void setAnonymize(const bool aAnonymize) = 0;
        
        ///
        /// Retreive wether the analytics service will be instructed to anonymize the user's IP address
        ///
        /// @return
        ///     Whether anonymization will be used
        ///
		 virtual bool isAnonymize() = 0;
        
        ///
        /// Set whether HTTPS will be used
        ///
        /// @param aUseHttps
        ///     Whether to use Https
        ///
		 virtual void setUseHttps(const bool aUseHttps) = 0;
        
        ///
        /// Retreive whether the tracker will use secure connection
        ///
        /// @return
        ///     Whether HTTPS will be used
        ///
		 virtual bool isUseHttps() = 0;
        
        ///
        /// Set the sample rate to instruct the analytics service to use
        ///
        /// @param aSampleRate
        ///     Sample rate to use
        ///
		 virtual void setSampleRate(const double aSampleRate) = 0;
        
        ///
        /// Retreive the sampling rate the analytics service will use
        ///
        /// @return
        ///     Sampling Rate
        ///
		 virtual double getSampleRate() = 0;
        
        ///
        /// Retreive the unique client ID used by this trcker
        ///
        /// @return
        ///     Client ID
        ///
		 virtual std::string getClientId() const = 0;
        
        ///
        /// Set the refferer url to send to the analytics servce
        ///
        /// @param aReferrerUrl
        ///     Referrer url
        ///
		 virtual void setReferrerUrl(const std::string& aReferrerUrl) = 0;
        
        ///
        /// Retreive the referrer url sent to the analytics service
        ///
        /// @return
        ///     Referrer URL
        ///
		 virtual std::string getReferrerUrl() = 0;
        
        ///
        /// Set the campaign url to send to the analytics servce
        ///
        /// @param aCampaignUrl
        ///     Campaign url
        ///
		 virtual void setCampaignUrl(const std::string& aCampaignUrl) = 0;
        
        ///
        /// Retreive the campaign url sent to the analytics service
        ///
        /// @return
        ///     Campaign URL
        ///
		 virtual std::string getCampaignUrl() = 0;
        
        ///
        /// Set the session timeout
        ///
        /// @param aSessionTimeout
        ///     Session Timeout
        ///
		 virtual void setSessionTimeout(const double aSessionTimeout) = 0;
        
        ///
        /// Retreive the session timeout
        ///
        /// @return
        ///     Session Timeout
        ///
		 virtual double getSessionTimeout() = 0;
        
	};
}

#endif
