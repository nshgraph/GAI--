//
//  test_hitbuilder.cpp
//  unittests
//
//  Created by Nathan Holmberg on 24/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "GAIDefines.h"
#include "Hit.h"
#include "HitBuilder.h"
#include "Model.h"


TEST( HitBuilderTest, create_valid_hits )
{
    GAI::Model model;
    GAI::Hit* hit;
    double timestamp;
    
    model.set(kAppNameModelKey, "appname");
    model.set(kTrackingIdModelKey, "tracking_id");
    model.set(kClientIdModelKey, "client_id");
    
    
    model.setForNextHit(kScreenParamModelKey, "screen");
    
    GAI::HitBuilder::createHit( GAI::kAppViewHit, model, hit );
    EXPECT_FALSE(hit == NULL);
    delete( hit );
    
    // but this should fail for a different type
    GAI::HitBuilder::createHit( GAI::kEventHit, model, hit );
    EXPECT_TRUE(hit == NULL);

    
    // but we can make the correct model for this type
    model.clearTemporaryValues();
    
    model.setForNextHit(kEventCategoryParamModelKey, "screen");
    model.setForNextHit(kEventActionParamModelKey, "screen");
    model.setForNextHit(kEventLabelParamModelKey, "screen");
    model.setForNextHit(kEventValueParamModelKey, "screen");
    
    GAI::HitBuilder::createHit( GAI::kEventHit, model, hit );
    EXPECT_FALSE(hit == NULL);
    
    // Validate the returned hit
    EXPECT_NE( hit->getDispatchURL(),"");
    EXPECT_NE(hit->getTimestamp(), 0);
    
    timestamp = hit->getTimestamp();
    
    delete( hit );
    
    // now create a new hit after a small timeout
    usleep(1000);
    GAI::HitBuilder::createHit( GAI::kEventHit, model, hit );
    EXPECT_FALSE(hit == NULL);
    
    // check that the hit has a timestamp later than the first hit
    EXPECT_GT( hit->getTimestamp(), timestamp);
    
}


TEST( HitBuilderTest, create_invalid_hits )
{
    GAI::Model model;
    GAI::Hit* hit;
    
    // an empty model fails
    GAI::HitBuilder::createHit( GAI::kAppViewHit, model, hit );
    EXPECT_TRUE(hit == NULL);
    
    // a model with the wrong parameters fails
    model.set(kSocialNetworkModelKey,"test");
    GAI::HitBuilder::createHit( GAI::kAppViewHit, model, hit );
    EXPECT_TRUE(hit == NULL);
    
}
