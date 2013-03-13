
#ifndef __Model_h__
#define __Model_h__

#include <map>
#include <string>

// This class stores parameters for use with sending a 'Hit'
// This inludes the functionality of storing a set of base values (that won't change between each use of the model)
// and a set of temporary values which can be cleared ready for the next use.


// model keys
#define kAppNameModelKey "appName"
#define kAppIdModelKey "appId"
#define kAppVersionModelKey "appVersion"
#define kTrackingIdModelKey "trackingId"
#define kSessionControlModelKey "sessionControl"
#define kAnonymizeIpModelKey "anonymizeIp"
#define kClientIdModelKey "clientId"
#define kReferrerModelKey "referrer"
#define kCampaignModelKey "campaign"
#define kSampleRateModelKey "sampleRate"


#define kScreenParamModelKey "screen"

#define kEventCategoryParamModelKey "eventCategory"
#define kEventActionParamModelKey "eventAction"
#define kEventLabelParamModelKey "eventLabel"
#define kEventValueParamModelKey "eventValue"

#define kTransationIdModelKey "transactionId"
#define kTransationAffiliationModelKey "transactionAffiliation"

#define kTransationItemNameModelKey "transactionItemName"
#define kTransationItemCodeModelKey "transactionItemCode"
#define kTransationItemCategoryModelKey "transactionItemCategory"
#define kTransationItemQuantityModelKey "transactionItemQuantity"
#define kTransationItemPriceModelKey "transactionItemPrice"

namespace GAI
{
	class Model
	{
    public:
        typedef std::map<std::string,std::string> ParameterMap;
    public:
        Model();
        std::string get(const std::string& key);
        Model::ParameterMap getKeysAndValues();
        
        void set(const std::string& key, const std::string& value);
        void setForNextHit(const std::string& key, const std::string& value);
        void setAll(const ParameterMap& parameters, bool isForNextHit);
        
        void clearTemporaryValues();
    private:
        ParameterMap values;
        ParameterMap temporaryValues;
	};
}

#endif
