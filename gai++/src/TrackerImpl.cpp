
#include "TrackerImpl.h"

#include <sstream>

#include "gai++/Transaction.h"
#include "gai++/TransactionItem.h"

#include "GAIDefines.h"
#include "Model.h"
#include "Dispatcher.h"
#include "HitBuilder.h"

#include "Platform.h"

namespace GAI {
    
    TrackerImpl::TrackerImpl(HitStore& aHitStore, const char* aClientID, const char* aTrackingID, const char* aAppName, const char* aAppVersion) :
    mHitStore( aHitStore ),
    mbTrackerOpen( true ),
    mbSessionStart( false ),
    mSessionTimeout(30)
    ///
    /// Constructor. Sets initial state only
    ///
    /// @param aHitStore
    ///     Place to send constructed hits
    /// @param aClientID
    ///     Unique ID for this client
    /// @param aTrackingID
    ///     Tracker ID for this tracker
    /// @param aAppName
    ///     App Name to send to analytics
    /// @param aAppVersion
    ///     App Verstion to send to analytics
    ///
    ///
    {
        mModel = new Model();
		mHitBuilder = new HitBuilder();
        setClientId( aClientID );
        setTrackingId( aTrackingID );
        setAppName( aAppName );
        setAppVersion( aAppVersion );
        // make sure the first hit sent indicates that this is the start of a new tracking session
        mModel->setForNextHit(kSessionControlModelKey, "start");
        mModel->set(kScreenResolutionModelKey, Platform::GetScreenResolution() );
        mModel->set(kUserLanguageModelKey, Platform::GetUserLanguage() );
    }
    
    TrackerImpl::~TrackerImpl()
    ///
    /// Destructor
    ///
    {
        if( mbTrackerOpen )
        {
            // send a final hit to end this session
            mModel->setForNextHit(kSessionControlModelKey, "end");
        
            ParameterMap parameters;
            parameters[kScreenParamModelKey] = "exit";
            internalSend(kAppViewHit, parameters);
        }
        
        delete mModel;
		delete mHitBuilder;
    }
    
    
    bool TrackerImpl::sendView(const char* aScreen, const CustomDimensionMap aDimensions, const CustomMetricMap aMetrics)
    ///
    /// Send a 'hit' representing the user viewing a screen
    ///
    /// @param aScreen
    ///     Name of the screen associated with this view
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        // ensure valid values have been provided
        if( std::string(aScreen) == "" )
            return false;
        // pack parameters
        ParameterMap parameters;
        parameters[kScreenParamModelKey] = aScreen;
        
        appendCustomParameters( parameters, aDimensions, aMetrics );
        
        return internalSend(kAppViewHit, parameters);
    }
    
    bool TrackerImpl::sendEvent(const char* aCategory, const char* aAction, const char* aLabel, const int& aValue, const CustomDimensionMap aDimensions, const CustomMetricMap aMetrics)
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
    {
        
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
		
        ParameterMap parameters;
        parameters[kEventCategoryParamModelKey] = aCategory;
        parameters[kEventActionParamModelKey] = aAction;
        parameters[kEventLabelParamModelKey] = aLabel;
        if( aValue != -1 )
		{
			std::ostringstream s;
			s << aValue;
            parameters[kEventValueParamModelKey] = s.str();
		}
        
        appendCustomParameters( parameters, aDimensions, aMetrics );
        
        return internalSend(kEventHit, parameters);
    }
    
    bool TrackerImpl::sendTransaction(const Transaction* aTransaction)
    ///
    /// Send a 'hit' representing a Transaction
    ///
    /// @param aTransaction
    ///     Transaction to send
    ///
    /// @return
    ///     Whether the operation was successful
    ///
    {
        bool success = true;
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        // ensure valid values have been provided
        if( !aTransaction )
            return false;
        
        ParameterMap transaction_parameters;
        transaction_parameters[kTransationIdModelKey] = aTransaction->getTransactionId();
        transaction_parameters[kTransationAffiliationModelKey] = aTransaction->getAffiliation();
        success &= internalSend(kTransactionHit, transaction_parameters);
        
        Transaction::TransactionItemList items = aTransaction->getTransactionItems();
        for( Transaction::TransactionItemList::const_iterator it = items.begin(), it_end = items.end(); it != it_end; it++)
        {
            ParameterMap item_parameters;
            item_parameters[kTransationIdModelKey] = aTransaction->getTransactionId();
            item_parameters[kTransationItemCodeModelKey] = (*it)->getProductCode();
            item_parameters[kTransationItemNameModelKey] = (*it)->getProductName();
            item_parameters[kTransationItemCategoryModelKey] = (*it)->getProductCategory();
            item_parameters[kTransationItemQuantityModelKey] = (*it)->getQuantity();
            item_parameters[kTransationItemPriceModelKey] = (*it)->getPrice();
            success &= internalSend(kTransactionItemHit, item_parameters);
        }
        
        return success;
    }
    
    bool TrackerImpl::sendException(const bool aIsFatal, const char* aDescription)
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
    {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        
        ParameterMap parameters;
        parameters[kExceptionDescriptionModelKey] = aDescription;
        parameters[kExceptionFatalModelKey] = aIsFatal ? "1" : "0";
        return internalSend(kExceptionHit,parameters);
    }
    
    bool TrackerImpl::sendTimingWithCategory(const char* aCategory, const double aTime, const char* aName, const char* aLabel)
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
    {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        ParameterMap parameters;
        parameters[kTimingCategoryModelKey] = aCategory;
        parameters[kTimingValueModelKey] = aTime;
        parameters[kTimingNameModelKey] = aName;
        parameters[kTimingLabelModelKey] = aLabel;
        return internalSend(kTimingHit,parameters);
    }
    
    bool TrackerImpl::sendSocial(const char* aNetwork, const char* aAction, const char* aTarget)
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
    {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        
        ParameterMap parameters;
        parameters[kSocialNetworkModelKey] = aNetwork;
        parameters[kSocialActionModelKey] = aAction;
        parameters[kSocialTargetModelKey] = aTarget;
        return internalSend(kSocialHit,parameters);
    }
    
    void TrackerImpl::close()
    ///
    /// Close the tracker. Any further events will be ignored
    ///
    {
        mbTrackerOpen = false;
    }
    
    void TrackerImpl::setTrackingId(const char* aTrackingId)
    ///
    /// Internal function to set the tracking ID. Will only work before the first Hit is sent
    ///
    /// @param aTrackingId
    ///     Tracking ID
    ///
    {
        if( !mbSessionStart && std::string(aTrackingId) != "")
            mModel->set(kTrackingIdModelKey, aTrackingId);
    }
    
    const char* TrackerImpl::getTrackingId() const
    ///
    /// Retreive the id of this tracker
    ///
    /// @return
    ///     Tracker ID
    ///
    {
        return mModel->get(kTrackingIdModelKey).c_str();
    }
    
    void TrackerImpl::setAppName(const char* aAppName)
    ///
    /// Set the App name reported by this tracker. Will only work before the first Hit is sent
    ///
    /// @param aAppName
    ///     App Name
    ///
    {
        if( !mbSessionStart && std::string(aAppName) != "")
            mModel->set(kAppNameModelKey, aAppName);
    }
    
    const char* TrackerImpl::getAppName() const
    ///
    /// Retreive the app name this tracker is tracking
    ///
    /// @return
    ///     App Name
    ///
    {
        return mModel->get(kAppNameModelKey).c_str();
    }
    
    void TrackerImpl::setAppId(const char* aAppId)
    ///
    /// Set the App ID reported by this tracker. Will only work before the first Hit is sent
    ///
    /// @param aAppId
    ///     App ID
    ///
    {
        if( !mbSessionStart && std::string(aAppId) != "")
            mModel->set(kAppIdModelKey, aAppId);
    }
    
    const char* TrackerImpl::getAppId() const
    ///
    /// Retreive the app id this tracker is tracking
    ///
    /// @return
    ///     App ID
    ///
    {
        return mModel->get(kAppIdModelKey).c_str();
    }
    
    void TrackerImpl::setAppVersion(const char* aAppVersion)
    ///
    /// Set the App version reported by this tracker. Will only work before the first Hit is sent
    ///
    /// @param aAppVersion
    ///     App Version
    ///
    {
        if( !mbSessionStart && std::string(aAppVersion) != "")
            mModel->set(kAppVersionModelKey, aAppVersion);
    }
    
    const char* TrackerImpl::getAppVersion() const
    ///
    /// Retreive the app version this tracker is tracking
    ///
    /// @return
    ///     App Version
    ///
    {
        return mModel->get(kAppVersionModelKey).c_str();
    }
    
    void TrackerImpl::setClientId(const char* aClientId)
    ///
    /// Internal function to set the client ID. Will only work before the first Hit is sent
    ///
    /// @param aClientId
    ///     Client ID
    ///
    {
        if( !mbSessionStart && std::string(aClientId) != "")
            mModel->set(kClientIdModelKey, aClientId);
    }
    
    const char* TrackerImpl::getClientId() const
    ///
    /// Retreive the unique client ID used by this trcker
    ///
    /// @return
    ///     Client ID
    ///
    {
        return mModel->get(kClientIdModelKey).c_str();
    }
    
    void TrackerImpl::setAnonymize(const bool aAnonymize)
    ///
    /// Set whether the user's IP address will be anonymized. Will only work before the first Hit is sent
    ///
    /// @param aAnonymize
    ///    Seetting for anonymization
    ///
    {
        if( aAnonymize )
            mModel->set(kAnonymizeIpModelKey, "1" );
        else
            mModel->remove(kAnonymizeIpModelKey);
    }
    
    bool TrackerImpl::isAnonymize()
    ///
    /// Retreive wether the analytics service will be instructed to anonymize the user's IP address
    ///
    /// @return
    ///     Whether anonymization will be used
    ///
    {
        return mModel->get(kAnonymizeIpModelKey) == "1";
    }
    
    void TrackerImpl::setSampleRate(const double aSampleRate)
    ///
    /// Set the sample rate to instruct the analytics service to use
    ///
    /// @param aSampleRate
    ///     Sample rate to use
    ///
    {
        this->mSampleRate = aSampleRate;
    }
    
    double TrackerImpl::getSampleRate()
    ///
    /// Retreive the sampling rate the analytics service will use
    ///
    /// @return
    ///     Sampling Rate
    ///
    {
        return this->mSampleRate;
    }
    
    
    void TrackerImpl::setReferrerUrl(const char* aReferrerUrl)
    ///
    /// Set the refferer url to send to the analytics servce
    ///
    /// @param aReferrerUrl
    ///     Referrer url
    ///
    {
        mModel->set(kReferrerModelKey, aReferrerUrl);
    }
    
    const char* TrackerImpl::getReferrerUrl()
    ///
    /// Retreive the referrer url sent to the analytics service
    ///
    /// @return
    ///     Referrer URL
    ///
    {
        return mModel->get(kReferrerModelKey).c_str();
    }
    
    void TrackerImpl::setCampaignUrl(const char* aCampaignUrl)
    ///
    /// Set the campaign url to send to the analytics servce
    ///
    /// @param aCampaignUrl
    ///     Campaign url
    ///
    {
        mModel->set(kCampaignModelKey, aCampaignUrl);
    }
    
    const char* TrackerImpl::getCampaignUrl()
    ///
    /// Retreive the campaign url sent to the analytics service
    ///
    /// @return
    ///     Campaign URL
    ///
    {
        return mModel->get(kCampaignModelKey).c_str();
    }
    
    void TrackerImpl::setSessionTimeout(const double aSessionTimeout)
    ///
    /// Set the session timeout
    ///
    /// @param aSessionTimeout
    ///     Session Timeout
    ///
    {
        this->mSessionTimeout = aSessionTimeout;
    }
    
    double TrackerImpl::getSessionTimeout()
    ///
    /// Retreive the session timeout
    ///
    /// @return
    ///     Session Timeout
    ///
    {
        return this->mSessionTimeout;
    }
    
    void TrackerImpl::setViewportSize( const char* aViewportSize )
    ///
    /// Set the viewport size to be sent
    ///
    /// @param
    ///     Viewport size
    ///
    {
        mModel->set(kViewportSizeModelKey, aViewportSize);
    }
    
    bool TrackerImpl::internalSend(const HitType aType, const ParameterMap& aParameters)
    ///
    /// Internal function to send a Hit based on parameters
    ///
    /// @param aType
    ///     Type of hit to send
    /// @param aParameters
    ///     Parameters to send for this hit.
    ///
    /// @return
    ///     Whether the operation was sucessful
    ///
    {
        
        tthread::lock_guard<tthread::mutex> lock(mModelLock);
        Hit* hit;
        
        // if we are sending then the session has started
        mbSessionStart = true;
        
        // add the parameters as temporaries to the model
        mModel->setAll(aParameters, true);
        //create a hit
        mHitBuilder->createHit(aType, *mModel, hit);
        //reset the temporary values
        mModel->clearTemporaryValues();
        
        // no hit means we failed
        if( !hit )
            return false;
        // send this hit
        return mHitStore.storeHit(*hit);
        
    }
    
    void TrackerImpl::appendCustomParameters( ParameterMap& aParameters, const CustomDimensionMap& aDimensions, const CustomMetricMap& aMetrics)
    ///
    /// Appends the custom dimensions and metrics to the parameter map
    ///
    /// @param aParameters
    ///     The parameter map to add dimensions and metrics to
    ///
    /// @param aDimensions
    ///     The dimensions (i.e. strings) to add along with their indices
    /// @param aMetrics
    ///     The metrics (i.e. numbers in string format) to add along with their indices
    ///
    {
        for( CustomDimensionMap::const_iterator it = aDimensions.begin(), it_end = aDimensions.end(); it != it_end; it++)
        {
            std::ostringstream key;
            key << kCustomDimensionPrefixKey << it->first;
            aParameters[key.str()] = it->second;
		}
		
		for( CustomMetricMap::const_iterator it = aMetrics.begin(), it_end = aMetrics.end(); it != it_end; it++)
		{
			std::ostringstream key;
			key << kCustomMetricPrefixKey << it->first;
			aParameters[key.str()] = it->second;
		}
    }
    
}
