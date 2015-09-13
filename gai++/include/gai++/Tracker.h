
#ifndef __Tracker_h__
#define __Tracker_h__

#include <string>
#include <map>

namespace GAI
{
	class Transaction;
    
    typedef std::map<int, std::string> CustomDimensionMap;
    typedef std::map<int, std::string> CustomMetricMap;
    
	class Tracker
    {
    public:
        virtual ~Tracker()
        ///
        /// Default Destructor
        ///
        {}
        
        ///
        /// Send a 'hit' representing the user viewing a screen
        ///
        /// @param aScreen
        ///     Name of the screen associated with this view
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendView( const char* view ) = 0;
        
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
        /// @param aDimensions
        ///     Any custom dimensions to be included in this hit
        /// @param aMetrics
        ///     Any custom metrics to be included in this hit
        ///
        /// @return
        ///     Whether the operation was successful
        ///
		 virtual bool sendEvent(const char* aCategory, const char* aAction, const char* aLabel, const int& aValue = -1, const CustomDimensionMap aDimensions = CustomDimensionMap(), const CustomMetricMap aMetrics = CustomMetricMap() ) = 0;
        
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
		 virtual bool sendException(const bool aIsFatal, const char* aDescription) = 0;
        
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
		 virtual bool sendTimingWithCategory(const char* aCategory, const double aTime, const char* aName, const char* aLabel) = 0;
        
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
		 virtual  bool sendSocial(const char* aNetwork, const char* aAction, const char* aTarget) = 0;

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
		 virtual const char* getTrackingId() const = 0;
        
        ///
        /// Set the App name reported by this tracker. Will only work before the first Hit is sent
        ///
        /// @param aAppName
        ///     App Name
        ///
		 virtual void setAppName(const char* aAppName) = 0;
        
        ///
        /// Retreive the app name this tracker is tracking
        ///
        /// @return
        ///     App Name
        ///
		 virtual const char* getAppName() const = 0;
        
        ///
        /// Set the App ID reported by this tracker. Will only work before the first Hit is sent
        ///
        /// @param aAppId
        ///     App ID
        ///
		 virtual void setAppId(const char* aAppId) = 0;
        
        ///
        /// Retreive the app id this tracker is tracking
        ///
        /// @return
        ///     App ID
        ///
		 virtual const char* getAppId() const = 0;
        
        ///
        /// Set the App version reported by this tracker. Will only work before the first Hit is sent
        ///
        /// @param aAppVersion
        ///     App Version
        ///
		 virtual void setAppVersion(const char* aAppVersion) = 0;
        
        ///
        /// Retreive the app version this tracker is tracking
        ///
        /// @return
        ///     App Version
        ///
		 virtual const char* getAppVersion() const = 0;
        
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
		 virtual const char* getClientId() const = 0;
        
        ///
        /// Set the refferer url to send to the analytics servce
        ///
        /// @param aReferrerUrl
        ///     Referrer url
        ///
		 virtual void setReferrerUrl(const char* aReferrerUrl) = 0;
        
        ///
        /// Retreive the referrer url sent to the analytics service
        ///
        /// @return
        ///     Referrer URL
        ///
		 virtual const char* getReferrerUrl() = 0;
        
        ///
        /// Set the campaign url to send to the analytics servce
        ///
        /// @param aCampaignUrl
        ///     Campaign url
        ///
		 virtual void setCampaignUrl(const char* aCampaignUrl) = 0;
        
        ///
        /// Retreive the campaign url sent to the analytics service
        ///
        /// @return
        ///     Campaign URL
        ///
        virtual const char* getCampaignUrl() = 0;
        
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
        
        ///
        /// Set the viewport size to be sent
        ///
        /// @param
        ///     Viewport size
        ///
        virtual void setViewportSize( const char* aViewportSize ) = 0;
        
	};
}

#endif
