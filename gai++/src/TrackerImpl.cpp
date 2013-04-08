
#include "TrackerImpl.h"

#include "GAIDefines.h"
#include "Model.h"
#include "Dispatcher.h"
#include "Transaction.h"
#include "TransactionItem.h"
#include "HitBuilder.h"

namespace GAI {
    
    TrackerImpl::TrackerImpl(HitStore& aHitStore, const std::string& aClientID, const std::string& aTrackingID, const std::string& aAppName, const std::string& aAppVersion) :
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
        setClientId( aClientID );
        setTrackingId( aTrackingID );
        setAppName( aAppName );
        setAppVersion( aAppVersion );
        // make sure the first hit sent indicates that this is the start of a new tracking session
        mModel->setForNextHit(kSessionControlModelKey, "start");
    }
    
    TrackerImpl::~TrackerImpl()
    ///
    /// Destructor
    ///
    {
        delete mModel;
    }
    
    
    bool TrackerImpl::sendView(const std::string& aScreen)
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
        if( aScreen == "" )
            return false;
        // pack parameters
        ParameterMap parameters;
        parameters[kScreenParamModelKey] = aScreen;
        return internalSend(kAppViewHit, parameters);
    }
    
    bool TrackerImpl::sendEvent(const std::string& aCategory, const std::string& aAction, const std::string& aLabel, const std::string& aValue)
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
        if( aValue != "" )
            parameters[kEventValueParamModelKey] = aValue;
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
    
    bool TrackerImpl::sendException(const bool aIsFatal, const std::string& aDescription)
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
    
    bool TrackerImpl::sendTimingWithCategory(const std::string& aCategory, const double aTime, const std::string& aName, const std::string& aLabel)
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
    
    bool TrackerImpl::sendSocial(const std::string& aNetwork, const std::string& aAction, const std::string& aTarget)
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
    
    void TrackerImpl::setTrackingId(const std::string& aTrackingId)
    ///
    /// Internal function to set the tracking ID. Will only work before the first Hit is sent
    ///
    /// @param aTrackingId
    ///     Tracking ID
    ///
    {
        if( !mbSessionStart && aTrackingId != "")
            mModel->set(kTrackingIdModelKey, aTrackingId);
    }
    
    std::string TrackerImpl::getTrackingId() const
    ///
    /// Retreive the id of this tracker
    ///
    /// @return
    ///     Tracker ID
    ///
    {
        return mModel->get(kTrackingIdModelKey);
    }
    
    void TrackerImpl::setAppName(const std::string& aAppName)
    ///
    /// Set the App name reported by this tracker. Will only work before the first Hit is sent
    ///
    /// @param aAppName
    ///     App Name
    ///
    {
        if( !mbSessionStart && aAppName != "")
            mModel->set(kAppNameModelKey, aAppName);
    }
    
    std::string TrackerImpl::getAppName() const
    ///
    /// Retreive the app name this tracker is tracking
    ///
    /// @return
    ///     App Name
    ///
    {
        return mModel->get(kAppNameModelKey);
    }
    
    void TrackerImpl::setAppId(const std::string& aAppId)
    ///
    /// Set the App ID reported by this tracker. Will only work before the first Hit is sent
    ///
    /// @param aAppId
    ///     App ID
    ///
    {
        if( !mbSessionStart && aAppId != "")
            mModel->set(kAppIdModelKey, aAppId);
    }
    
    std::string TrackerImpl::getAppId() const
    ///
    /// Retreive the app id this tracker is tracking
    ///
    /// @return
    ///     App ID
    ///
    {
        return mModel->get(kAppIdModelKey);
    }
    
    void TrackerImpl::setAppVersion(const std::string& aAppVersion)
    ///
    /// Set the App version reported by this tracker. Will only work before the first Hit is sent
    ///
    /// @param aAppVersion
    ///     App Version
    ///
    {
        if( !mbSessionStart && aAppVersion != "")
            mModel->set(kAppVersionModelKey, aAppVersion);
    }
    
    std::string TrackerImpl::getAppVersion() const
    ///
    /// Retreive the app version this tracker is tracking
    ///
    /// @return
    ///     App Version
    ///
    {
        return mModel->get(kAppVersionModelKey);
    }
    
    void TrackerImpl::setClientId(const std::string& aClientId)
    ///
    /// Internal function to set the client ID. Will only work before the first Hit is sent
    ///
    /// @param aClientId
    ///     Client ID
    ///
    {
        if( !mbSessionStart && aClientId != "")
            mModel->set(kClientIdModelKey, aClientId);
    }
    
    std::string TrackerImpl::getClientId() const
    ///
    /// Retreive the unique client ID used by this trcker
    ///
    /// @return
    ///     Client ID
    ///
    {
        return mModel->get(kClientIdModelKey);
    }
    
    void TrackerImpl::setAnonymize(const bool aAnonymize)
    ///
    /// Set whether the user's IP address will be anonymized. Will only work before the first Hit is sent
    ///
    /// @param aAnonymize
    ///    Seetting for anonymization
    ///
    {
        mModel->set(kAnonymizeIpModelKey, aAnonymize ? "1" : "0");
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
    
    void TrackerImpl::setUseHttps(const bool aUseHttps)
    ///
    /// Set whether HTTPS will be used
    ///
    /// @param aUseHttps
    ///     Whether to use Https
    ///
    {
        this->mbUseHttps = aUseHttps;
    }
    
    bool TrackerImpl::isUseHttps()
    ///
    /// Retreive whether the tracker will use secure connection
    ///
    /// @return
    ///     Whether HTTPS will be used
    ///
    {
        return this->mbUseHttps;
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
    
    
    void TrackerImpl::setReferrerUrl(const std::string& aReferrerUrl)
    ///
    /// Set the refferer url to send to the analytics servce
    ///
    /// @param aReferrerUrl
    ///     Referrer url
    ///
    {
        mModel->set(kReferrerModelKey, aReferrerUrl);
    }
    
    std::string TrackerImpl::getReferrerUrl()
    ///
    /// Retreive the referrer url sent to the analytics service
    ///
    /// @return
    ///     Referrer URL
    ///
    {
        return mModel->get(kReferrerModelKey);
    }
    
    void TrackerImpl::setCampaignUrl(const std::string& aCampaignUrl)
    ///
    /// Set the campaign url to send to the analytics servce
    ///
    /// @param aCampaignUrl
    ///     Campaign url
    ///
    {
        mModel->set(kCampaignModelKey, aCampaignUrl);
    }
    
    std::string TrackerImpl::getCampaignUrl()
    ///
    /// Retreive the campaign url sent to the analytics service
    ///
    /// @return
    ///     Campaign URL
    ///
    {
        return mModel->get(kCampaignModelKey);
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
        HitBuilder::createHit( aType, *mModel, hit );
        //reset the temporary values
        mModel->clearTemporaryValues();
        
        // no hit means we failed
        if( !hit )
            return false;
        // send this hit
        return mHitStore.storeHit(*hit);
        
    }
    
}
