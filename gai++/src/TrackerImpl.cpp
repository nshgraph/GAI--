
#include "TrackerImpl.h"

#include "Model.h"
#include "Dispatcher.h"
#include "Transaction.h"
#include "TransactionItem.h"
#include "ClientID.h"

namespace GAI {
    
    TrackerImpl::TrackerImpl(Dispatcher* aDispatcher, const std::string& aTrackingID, const std::string& aAppName, const std::string& aAppVersion) :
    mDispatcher( aDispatcher ),
    mbTrackerOpen( true ),
    mSessionTimeout(30)
    {
        mModel = new Model();
        setClientId( ClientID::generateClientID() );
        setTrackingId( aTrackingID );
        setAppName( aAppName );
        setAppVersion( aAppVersion );
        // make sure the first hit sent indicates that this is the start of a new tracking session
        mModel->setForNextHit(kSessionControlModelKey, "start");
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
        internalSend(kAppViewHit, parameters);
        
        return true;
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
        internalSend(kEventHit, parameters);
        return true;
    }
    
    bool TrackerImpl::sendTransaction(const Transaction* aTransaction) {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        // ensure valid values have been provided
        if( !aTransaction )
            return false;
        
        ParameterMap transaction_parameters;
        transaction_parameters[kTransationIdModelKey] = aTransaction->getTransactionId();
        transaction_parameters[kTransationAffiliationModelKey] = aTransaction->getAffiliation();
        internalSend(kTransactionHit, transaction_parameters);
        
        Transaction::TransactionItemList items = aTransaction->getTransactionItems();
        for( Transaction::TransactionItemList::const_iterator it = items.begin(), it_end = items.end(); it != it_end; it++)
        {
            ParameterMap item_parameters;
            transaction_parameters[kTransationIdModelKey] = aTransaction->getTransactionId();
            transaction_parameters[kTransationItemCodeModelKey] = (*it)->getProductCode();
            transaction_parameters[kTransationItemNameModelKey] = (*it)->getProductName();
            transaction_parameters[kTransationItemCategoryModelKey] = (*it)->getProductCategory();
            transaction_parameters[kTransationItemQuantityModelKey] = (*it)->getQuantity();
            transaction_parameters[kTransationItemPriceModelKey] = (*it)->getPrice();
            internalSend(kTransactionItemHit, item_parameters);
        }
        
        return true;
    }
    
    bool TrackerImpl::sendException(const bool aIsFatal, const std::string& aDescription) {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        return true;
    }
    
    bool TrackerImpl::sendTimingWithCategory(const std::string& aCategory, const double aTime, const std::string& aName, const std::string& aLabel) {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        return true;
    }
    
    bool TrackerImpl::sendSocial(const std::string& aNetwork, const std::string& aAction, const std::string& aTarget) {
        // ensure tracker is open
        if( !mbTrackerOpen )
            return false;
        return true;
    }
    
    bool TrackerImpl::setParameter(const std::string& aName, const std::string& aValue) {
        throw "Not yet implemented";
    }
    
    std::string TrackerImpl::getParameter(const std::string& aName) const{
        throw "Not yet implemented";
    }
    
    bool TrackerImpl::sendParameters(const std::string& aTrackType, const std::map<std::string, std::string>& aParameters) {
        throw "Not yet implemented";
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
        mModel->set(kAnonymizeIpModelKey, aAnonymize ? "true" : "false");
    }
    
    bool TrackerImpl::isAnonymize()
    {
        return mModel->get(kAnonymizeIpModelKey) == "true";
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
    
    
    bool TrackerImpl::internalSend(HitType aType, ParameterMap aParameters) {
        // if we are sending then the session has started
        mbSessionStart = true;
        throw "Not yet implemented";
    }
    
}
