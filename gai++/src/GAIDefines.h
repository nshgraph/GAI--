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
#define kAppNameModelKey				"an"
#define kAppIdModelKey					"aid"
#define kAppVersionModelKey				"av"
#define kTrackingIdModelKey				"tid"
#define kHitTypeModelKey				"t"
#define kSessionControlModelKey			"sc"
#define kAnonymizeIpModelKey			"aip"
#define kClientIdModelKey				"cid"
#define kReferrerModelKey				"dr"
#define kCampaignModelKey				"cn"
#define kSampleRateModelKey				"sf"
#define kQueueTimeModelKey				"qt"
#define kCacheBusterModelKey			"z"
#define kScreenResolutionModelKey		"sr"
#define kUserLanguageModelKey			"ul"
#define kViewportSizeModelKey			"vp"

///
/// Hit Type Keys
///
#define kHitTypeAppViewValue			"appview"
#define kHitTypeEventValue				"event"
#define kHitTypeTransactionValue		"transaction"
#define kHitTypeTransactionItemValue	"item"
#define kHitTypeExceptionValue			"exception"
#define kHitTypeTimingValue				"timing"
#define kHitTypeSocialValue				"social"

///
/// AppView Keys
///
#define kScreenParamModelKey			"cd"

///
/// Event Keys
///
#define kEventCategoryParamModelKey		"ec"
#define kEventActionParamModelKey		"ea"
#define kEventLabelParamModelKey		"el"
#define kEventValueParamModelKey		"ev"

///
/// Transaction Keys
///
#define kTransationIdModelKey			"ti"
#define kTransationAffiliationModelKey	"ta"

///
/// Transaction Item Keys
///
#define kTransationItemNameModelKey		"in"
#define kTransationItemCodeModelKey		"ic"
#define kTransationItemCategoryModelKey "iv"
#define kTransationItemQuantityModelKey "iq"
#define kTransationItemPriceModelKey	"ip"

///
/// Exception Keys
///
#define kExceptionDescriptionModelKey	"exd"
#define kExceptionFatalModelKey			"exf"

///
/// Timing Keys
///
#define kTimingCategoryModelKey			"utc"
#define kTimingValueModelKey			"utt"
#define kTimingNameModelKey				"utv"
#define kTimingLabelModelKey			"utl"

///
/// Social Keys
///
#define kSocialNetworkModelKey			"sn"
#define kSocialActionModelKey			"sa"
#define kSocialTargetModelKey			"st"

#endif
