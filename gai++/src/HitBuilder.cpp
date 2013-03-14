#include <exception>
#include <string>
#include <list>


#include "HitBuilder.h"
#include "Hit.h"
#include "Model.h"
#include "HitType.h"

namespace GAI {


Hit* HitBuilder::createHit(HitType aType, Model* aModel) {
    bool hasRequired = true;
    const std::list<std::string>& requirements = requiredParametersForType(aType);
    for( std::list<std::string>::const_iterator it = requirements.begin(), it_end = requirements.end(); it != it_end && hasRequired;it++)
    {
        if( !aModel->hasParameter( *it) )
            hasRequired = false;
    }
    
    Hit* result = NULL;
    if( hasRequired )
    {
        result = new Hit();
        result->setParameters(aModel->getKeysAndValues());
        //TODO: Use some part of libevent to retrieve the current time
        //result->setTimestamp(System.currentTimeMillis(););
    }
    return result;
}

const std::list<std::string>& HitBuilder::requiredParametersForType(const HitType aType) {
    static std::list<std::string> requirements[kNumHitTypes];
    static bool initialized = false;
    if(!initialized)
    {
        initialized = true;
        
        // First register all parameters that are needed regardless of hit type
        for( int i=0;i<kNumHitTypes;i++)
        {
            requirements[i].push_back(kAppNameModelKey);
            requirements[i].push_back(kTrackingIdModelKey);
            requirements[i].push_back(kClientIdModelKey);
        }
        // Now register each individual hit type's requirements
        requirements[kAppViewHit].push_back(kScreenParamModelKey);
        
        requirements[kEventHit].push_back(kEventCategoryParamModelKey);
        requirements[kEventHit].push_back(kEventActionParamModelKey);
        requirements[kEventHit].push_back(kEventLabelParamModelKey);
        requirements[kEventHit].push_back(kEventValueParamModelKey);
        
        requirements[kTransactionHit].push_back(kTransationIdModelKey);
        requirements[kTransactionHit].push_back(kTransationAffiliationModelKey);
        
        requirements[kTransactionItemHit].push_back(kTransationIdModelKey);
        requirements[kTransactionItemHit].push_back(kTransationItemNameModelKey);
        requirements[kTransactionItemHit].push_back(kTransationItemCodeModelKey);
        requirements[kTransactionItemHit].push_back(kTransationItemCategoryModelKey);
        requirements[kTransactionItemHit].push_back(kTransationItemQuantityModelKey);
        requirements[kTransactionItemHit].push_back(kTransationItemPriceModelKey);
        
        requirements[kExceptionHit].push_back(kExceptionDescriptionModelKey);
        requirements[kExceptionHit].push_back(kExceptionFatalModelKey);
        
        requirements[kTimingHit].push_back(kTimingCategoryModelKey);
        requirements[kTimingHit].push_back(kTimingValueModelKey);
        requirements[kTimingHit].push_back(kTimingNameModelKey);
        requirements[kTimingHit].push_back(kTimingLabelModelKey);
        
        requirements[kSocialHit].push_back(kSocialNetworkModelKey);
        requirements[kSocialHit].push_back(kSocialActionModelKey);
        requirements[kSocialHit].push_back(kSocialTargetModelKey);
        
    }
    
    return requirements[aType];
    
}

}