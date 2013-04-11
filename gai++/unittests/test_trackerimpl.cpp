//
//  test_trackerimpl.cpp
//  unittests
//
//  Created by Nathan Holmberg on 25/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "TrackerImpl.h"
#include "HitStore.h"
#include "Transaction.h"


class FakeHitStore : public GAI::HitStore
{
public:
    FakeHitStore() :
    mNumHits(0)
    {
        
    }
    bool storeHit( const GAI::Hit& hit )
    {
        mNumHits++;
        return true;
    }
    int getNumHits()
    {
        return mNumHits;
    }
private:
    int mNumHits;
};


TEST( TrackerImplTest, send_types )
{
    FakeHitStore dispatch = FakeHitStore();
    GAI::TrackerImpl tracker = GAI::TrackerImpl(dispatch,"clientID","trackingID","appName","appVersion");
    
    EXPECT_EQ(dispatch.getNumHits(),0);
    
    // attempt to send a view
    EXPECT_TRUE( tracker.sendView("screen") );
    EXPECT_EQ(dispatch.getNumHits(),1);
    
    EXPECT_FALSE( tracker.sendView("") );
    EXPECT_EQ(dispatch.getNumHits(),1);
    
    // attempt to send an event
    EXPECT_TRUE( tracker.sendEvent("category","action","label","value") );
    EXPECT_EQ(dispatch.getNumHits(),2);
    
    // attempt to send an exception
    EXPECT_TRUE( tracker.sendException(false,"exception") );
    EXPECT_EQ(dispatch.getNumHits(),3);
    EXPECT_TRUE( tracker.sendException(true,"exception") );
    EXPECT_EQ(dispatch.getNumHits(),4);
    
    GAI::Transaction* transaction = GAI::Transaction::createTransaction("id", "affiliation");
    EXPECT_TRUE( tracker.sendTransaction(transaction) );
    EXPECT_EQ(dispatch.getNumHits(),5);
    delete( transaction );
    
    // attempt to send timing
    EXPECT_TRUE( tracker.sendTimingWithCategory("category", 1.0, "name", "label" ) );
    EXPECT_EQ(dispatch.getNumHits(),6);
    
    // attempt to send social
    EXPECT_TRUE( tracker.sendSocial("network","action","target") );
    EXPECT_EQ(dispatch.getNumHits(),7);
    
    // can close
    tracker.close();
    
    EXPECT_FALSE( tracker.sendSocial("network","action","target") );
    EXPECT_EQ(dispatch.getNumHits(),7);
}

TEST( TrackerImplTest, get_and_set)
{
    FakeHitStore dispatch = FakeHitStore();
    std::string clientID = "clientID";
    std::string trackingID = "trackingID";
    std::string appName = "appName";
    std::string appName2 = "appName2";
    std::string appVersion = "appVersion";
    std::string appVersion2 = "appVersion2";
    std::string appID = "appID";
    std::string appID2 = "appID2";
    GAI::TrackerImpl tracker = GAI::TrackerImpl(dispatch,clientID,trackingID,appName,appVersion);
    
    // can get tracker id
    EXPECT_EQ( tracker.getTrackingId(), trackingID );
    
    // can get tracker id
    EXPECT_EQ( tracker.getClientId(), clientID );
    
    // can get and set app name
    EXPECT_EQ( tracker.getAppName(), appName );
    
    tracker.setAppName(appName2);
    EXPECT_EQ( tracker.getAppName(), appName2 );
    
    // can get and set app version
    EXPECT_EQ( tracker.getAppVersion(), appVersion );
    
    tracker.setAppVersion(appVersion2);
    EXPECT_EQ( tracker.getAppVersion(), appVersion2 );
    
    // can get and set app ID
    tracker.setAppId(appID);
    EXPECT_EQ( tracker.getAppId(), appID );
    
    // can get and set anonymize
    tracker.setAnonymize(false);
    EXPECT_EQ( tracker.isAnonymize(), false );
    tracker.setAnonymize(true);
    EXPECT_EQ( tracker.isAnonymize(), true );
    
    // can get and set sample rate
    tracker.setSampleRate(1.0);
    EXPECT_EQ( tracker.getSampleRate(), 1.0 );
    tracker.setSampleRate(2.0);
    EXPECT_EQ( tracker.getSampleRate(), 2.0 );
    
    // can get and set sample rate
    tracker.setSessionTimeout(1.0);
    EXPECT_EQ( tracker.getSessionTimeout(), 1.0 );
    tracker.setSessionTimeout(2.0);
    EXPECT_EQ( tracker.getSessionTimeout(), 2.0 );
    
    // can get and set refferer
    tracker.setReferrerUrl("referrer");
    EXPECT_EQ( tracker.getReferrerUrl(), "referrer" );
    
    // can get and set campaignid
    tracker.setCampaignUrl("campaign");
    EXPECT_EQ( tracker.getCampaignUrl(), "campaign" );
    
    // if we create a hit and start a session we can't change the app info anymore
    EXPECT_TRUE( tracker.sendView("screen") );
    
    
    tracker.setAppVersion(appVersion);
    EXPECT_EQ( tracker.getAppVersion(), appVersion2 ); // no change!
    
    tracker.setAppName(appName);
    EXPECT_EQ( tracker.getAppName(), appName2 ); // no change!
    
    tracker.setAppId(appID2);
    EXPECT_EQ( tracker.getAppId(), appID ); // no change!
    

    
}