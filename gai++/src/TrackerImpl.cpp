
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
    {
        delete mModel;
    }
    
    
    bool TrackerImpl::sendView(const std::string& aScreen) {
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
    
    bool TrackerImpl::sendEvent(const std::string& aCategory, const std::string& aAction, const std::string& aLabel, const std::string& aValue) {
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
    
    bool TrackerImpl::sendTransaction(const Transaction* aTransaction) {
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
    
    bool TrackerImpl::sendException(const bool aIsFatal, const std::string& aDescription) {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        
        ParameterMap parameters;
        parameters[kExceptionDescriptionModelKey] = aDescription;
        parameters[kExceptionFatalModelKey] = aIsFatal ? "1" : "0";
        return internalSend(kExceptionHit,parameters);
    }
    
    bool TrackerImpl::sendTimingWithCategory(const std::string& aCategory, const double aTime, const std::string& aName, const std::string& aLabel) {
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
    
    bool TrackerImpl::sendSocial(const std::string& aNetwork, const std::string& aAction, const std::string& aTarget) {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        
        ParameterMap parameters;
        parameters[kSocialNetworkModelKey] = aNetwork;
        parameters[kSocialActionModelKey] = aAction;
        parameters[kSocialTargetModelKey] = aTarget;
        return internalSend(kSocialHit,parameters);
    }
    
    void TrackerImpl::close() {
        mbTrackerOpen = false;
    }
    
    void TrackerImpl::setTrackingId(const std::string& aTrackingId)
    {
        if( !mbSessionStart && aTrackingId != "")
            mModel->set(kTrackingIdModelKey, aTrackingId);
    }
    
    std::string TrackerImpl::getTrackingId() const
    {
        return mModel->get(kTrackingIdModelKey);
    }
    
    void TrackerImpl::setAppName(const std::string& aAppName)
    {
        if( !mbSessionStart && aAppName != "")
            mModel->set(kAppNameModelKey, aAppName);
    }
    
    std::string TrackerImpl::getAppName() const
    {
        return mModel->get(kAppNameModelKey);
    }
    
    void TrackerImpl::setAppId(const std::string& aAppId)
    {
        if( !mbSessionStart && aAppId != "")
            mModel->set(kAppIdModelKey, aAppId);
    }
    
    std::string TrackerImpl::getAppId() const
    {
        return mModel->get(kAppIdModelKey);
    }
    
    void TrackerImpl::setAppVersion(const std::string& aAppVersion)
    {
        if( !mbSessionStart && aAppVersion != "")
            mModel->set(kAppVersionModelKey, aAppVersion);
    }
    
    std::string TrackerImpl::getAppVersion() const
    {
        return mModel->get(kAppVersionModelKey);
    }
    
    void TrackerImpl::setClientId(const std::string& aClientId)
    {
        if( !mbSessionStart && aClientId != "")
            mModel->set(kClientIdModelKey, aClientId);
    }
    
    std::string TrackerImpl::getClientId() const
    {
        return mModel->get(kClientIdModelKey);
    }
    
    void TrackerImpl::setAnonymize(const bool aAnonymize)
    {
        mModel->set(kAnonymizeIpModelKey, aAnonymize ? "1" : "0");
    }
    
    bool TrackerImpl::isAnonymize()
    {
        return mModel->get(kAnonymizeIpModelKey) == "1";
    }
    
    void TrackerImpl::setUseHttps(const bool aUseHttps)
    {
        this->mbUseHttps = aUseHttps;
    }
    
    bool TrackerImpl::isUseHttps()
    {
        return this->mbUseHttps;
    }
    
    void TrackerImpl::setSampleRate(const double aSampleRate)
    {
        this->mSampleRate = aSampleRate;
    }
    
    double TrackerImpl::getSampleRate()
    {
        return this->mSampleRate;
    }
    
    
    void TrackerImpl::setReferrerUrl(const std::string& aReferrerUrl)
    {
        mModel->set(kReferrerModelKey, aReferrerUrl);
    }
    
    std::string TrackerImpl::getReferrerUrl()
    {
        return mModel->get(kReferrerModelKey);
    }
    
    void TrackerImpl::setCampaignUrl(const std::string& aCampaignUrl)
    {
        mModel->set(kCampaignModelKey, aCampaignUrl);
    }
    
    std::string TrackerImpl::getCampaignUrl()
    {
        return mModel->get(kCampaignModelKey);
    }
    
    void TrackerImpl::setSessionTimeout(const double aSessionTimeout)
    {
        this->mSessionTimeout = aSessionTimeout;
    }
    
    double TrackerImpl::getSessionTimeout()
    {
        return this->mSessionTimeout;
    }
    
    
    bool TrackerImpl::internalSend(const HitType aType, const ParameterMap& aParameters) {
        // if we are sending then the session has started
        mbSessionStart = true;
        
        // add the parameters as temporaries to the model
        mModel->setAll(aParameters, true);
        //create a hit
        Hit* hit = HitBuilder::createHit( aType, *mModel );
        //reset the temporary values
        mModel->clearTemporaryValues();
        // no hit means we failed
        if( !hit )
            return false;
        // send this hit
        return mHitStore.storeHit(*hit);
        
    }
    
}
