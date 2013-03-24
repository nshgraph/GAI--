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
    
    model.set(kAppNameModelKey, "appname");
    model.set(kTrackingIdModelKey, "tracking_id");
    model.set(kClientIdModelKey, "client_id");
    
    
    model.setForNextHit(kScreenParamModelKey, "screen");
    
    hit = GAI::HitBuilder::createHit( GAI::kAppViewHit, model );
    EXPECT_FALSE(hit == NULL);
    delete( hit );
    
    // but this should fail for a different type
    hit = GAI::HitBuilder::createHit( GAI::kEventHit, model );
    EXPECT_TRUE(hit == NULL);

    
    // but we can make the correct model for this type
    model.clearTemporaryValues();
    
    model.setForNextHit(kEventCategoryParamModelKey, "screen");
    model.setForNextHit(kEventActionParamModelKey, "screen");
    model.setForNextHit(kEventLabelParamModelKey, "screen");
    model.setForNextHit(kEventValueParamModelKey, "screen");
    
    hit = GAI::HitBuilder::createHit( GAI::kEventHit, model );
    EXPECT_FALSE(hit == NULL);
    delete( hit );
    
}


TEST( HitBuilderTest, create_invalid_hits )
{
    GAI::Model model;
    GAI::Hit* hit;
    
    // an empty model fails
    hit = GAI::HitBuilder::createHit( GAI::kAppViewHit, model );
    EXPECT_TRUE(hit == NULL);
    
    // a model with the wrong parameters fails
    model.set(kSocialNetworkModelKey,"test");
    hit = GAI::HitBuilder::createHit( GAI::kAppViewHit, model );
    EXPECT_TRUE(hit == NULL);
    
}
