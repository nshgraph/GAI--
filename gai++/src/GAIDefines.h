//
//  GAIDefines.h
//  gai
//
//  Created by Steve Hosking on 22/03/13.
//
//

#ifndef gai_GAIDefines_h
#define gai_GAIDefines_h

#define kDispatchInterval	120 // 120 seconds (2 minutes)
#define kDispatchBlockSize  10
#define kOptOut				0 // On

#define kGAIVersion			"1"
#define kGAIURLHTTP         "www.google-analytics.com"
#define kGAIURLHTTPS        "ssl.google-analytics.com"
#define kGAIURLPage         "/collect"
#define kGAIPort            80

///
/// The following keys are used when adding information to a model
///
#define kAppNameModelKey "an"
#define kAppIdModelKey "aid"
#define kAppVersionModelKey "av"
#define kTrackingIdModelKey "tid"
#define kHitTypeModelKey "t"
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


#define kHitTypeAppViewValue "appview"
#define kHitTypeEventValue "event"
#define kHitTypeTransactionValue "transaction"
#define kHitTypeTransactionItemValue "item"
#define kHitTypeExceptionValue "exception"
#define kHitTypeTimingValue "timing"
#define kHitTypeSocialValue "social"


#endif
