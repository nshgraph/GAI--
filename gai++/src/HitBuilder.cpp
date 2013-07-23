#include <exception>
#include <string>
#include <list>

#include "GAIDefines.h"

#include "HitBuilder.h"
#include "Hit.h"
#include "Model.h"
#include "HitType.h"
#include "Timestamp.h"

namespace GAI {
    
	std::list<std::string> HitBuilder::sRequirements[kNumHitTypes];
	bool HitBuilder::sRequirementsInitialized = false;
    
    bool HitBuilder::createHit(const HitType aType, const Model& aModel, Hit*& hit)
    ///
    /// This function attempts to create a Hit by validating that the paramenters in the proided Model meet the requirements
    /// of the hit type. Returns either an allocated hit or NULL if fails.
    /// It is the responsibility of the caller to deallocate the Hit
    ///
    /// @param aType
    ///     The type of hit that is being attempted
    /// @param aModel
    ///     The Model containing the parameters to be associated with the hit
    ///
    /// @return
    ///     Either the allocated hit or NULL
    ///
    {
        bool hasRequired = true;
        const std::list<std::string>& requirements = requiredParametersForType(aType);
        for( std::list<std::string>::const_iterator it = requirements.begin(), it_end = requirements.end(); it != it_end && hasRequired;it++)
        {
            if( !aModel.hasParameter( *it) )
                hasRequired = false;
        }
        Model::ParameterMap params = aModel.getKeysAndValues();
        
        // insert the correct hit type
        switch( aType )
        {
            case kAppViewHit:
                params[kHitTypeModelKey] = kHitTypeAppViewValue;
                break;
            case kEventHit:
                params[kHitTypeModelKey] = kHitTypeEventValue;
                break;
            case kTransactionHit:
                params[kHitTypeModelKey] = kHitTypeTransactionValue;
                break;
            case kTransactionItemHit:
                params[kHitTypeModelKey] = kHitTypeTransactionItemValue;
                break;
            case kExceptionHit:
                params[kHitTypeModelKey] = kHitTypeExceptionValue;
                break;
            case kTimingHit:
                params[kHitTypeModelKey] = kHitTypeTimingValue;
                break;
            case kSocialHit:
                params[kHitTypeModelKey] = kHitTypeSocialValue;
                break;
            default:
                hasRequired = false;
                break;
        }
        
        hit = NULL;
        if( hasRequired )
        {
            hit = new Hit();
            hit->setParameters(params);
            hit->setTimestamp( Timestamp::generateTimestamp());
        }
        return (hit!=NULL);
    }
    
    const std::list<std::string>& HitBuilder::requiredParametersForType(const HitType aType)
    ///
    /// Retrieves a list of parameters that are needed for a HitType.
    ///
    /// @param aType
    ///     The type we are checking for
    ///
    /// @return
    ///     A list of all the required parameters that must be set for this HitType
    ///
    {
        if(!sRequirementsInitialized)
        {
            sRequirementsInitialized = true;
            
            // First register all parameters that are needed regardless of hit type
            for( int i=0;i<kNumHitTypes;i++)
            {
                sRequirements[i].push_back(kAppNameModelKey);
                sRequirements[i].push_back(kTrackingIdModelKey);
                sRequirements[i].push_back(kClientIdModelKey);
            }
            // Now register each individual hit type's requirements
            sRequirements[kAppViewHit].push_back(kScreenParamModelKey);
            
            sRequirements[kEventHit].push_back(kEventCategoryParamModelKey);
            sRequirements[kEventHit].push_back(kEventActionParamModelKey);
            
            sRequirements[kTransactionHit].push_back(kTransationIdModelKey);
            sRequirements[kTransactionHit].push_back(kTransationAffiliationModelKey);
            
            sRequirements[kTransactionItemHit].push_back(kTransationIdModelKey);
            sRequirements[kTransactionItemHit].push_back(kTransationItemNameModelKey);
            sRequirements[kTransactionItemHit].push_back(kTransationItemCodeModelKey);
            sRequirements[kTransactionItemHit].push_back(kTransationItemCategoryModelKey);
            sRequirements[kTransactionItemHit].push_back(kTransationItemQuantityModelKey);
            sRequirements[kTransactionItemHit].push_back(kTransationItemPriceModelKey);
            
            sRequirements[kExceptionHit].push_back(kExceptionDescriptionModelKey);
            sRequirements[kExceptionHit].push_back(kExceptionFatalModelKey);
            
            sRequirements[kTimingHit].push_back(kTimingCategoryModelKey);
            sRequirements[kTimingHit].push_back(kTimingValueModelKey);
            sRequirements[kTimingHit].push_back(kTimingNameModelKey);
            sRequirements[kTimingHit].push_back(kTimingLabelModelKey);
            
            sRequirements[kSocialHit].push_back(kSocialNetworkModelKey);
            sRequirements[kSocialHit].push_back(kSocialActionModelKey);
            sRequirements[kSocialHit].push_back(kSocialTargetModelKey);
            
        }
        
        return sRequirements[aType];
        
    }
    
}