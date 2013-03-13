
#ifndef __Model_h__
#define __Model_h__

#include <map>
#include <string>

// This class stores parameters for use with sending a 'Hit'
// This inludes the functionality of storing a set of base values (that won't change between each use of the model)
// and a set of temporary values which can be cleared ready for the next use.


// model keys
#define kAppNameModelKey "an"
#define kAppIdModelKey "aid"
#define kAppVersionModelKey "av"
#define kTrackingIdModelKey "tid"
#define kSessionControlModelKey "sc"
#define kAnonymizeIpModelKey "aip"
#define kClientIdModelKey "cid"
#define kReferrerModelKey "dr"
#define kCampaignModelKey "cn"
#define kSampleRateModelKey "sf"


#define kScreenParamModelKey "cd"

#define kEventCategoryParamModelKey "ec"
#define kEventActionParamModelKey "ea"
#define kEventLabelParamModelKey "el"
#define kEventValueParamModelKey "ev"

#define kTransationIdModelKey "ti"
#define kTransationAffiliationModelKey "ta"

#define kTransationItemNameModelKey "in"
#define kTransationItemCodeModelKey "ic"
#define kTransationItemCategoryModelKey "iv"
#define kTransationItemQuantityModelKey "iq"
#define kTransationItemPriceModelKey "ip"

#define kExceptionDescriptionModelKey "exd"
#define kExceptionFatalModelKey "exf"

#define kTimingCategoryModelKey "utc"
#define kTimingValueModelKey "utt"
#define kTimingNameModelKey "utv"
#define kTimingLabelModelKey "utl"

#define kSocialNetworkModelKey "sn"
#define kSocialActionModelKey "sa"
#define kSocialTargetModelKey "st"

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
