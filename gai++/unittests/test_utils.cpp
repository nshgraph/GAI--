//
//  test_utils.cpp
//  unittests
//
//  Created by Nathan Holmberg on 25/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"
#include "utilities.h"

#include "ClientID.h"
#include "Timestamp.h"
#include "DataStoreSqlite.h"

TEST( ClientIDTest, create_clientID )
{
    // create the datastore that client id needs
    const std::string test_db = "test.db";
    int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
    
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_EQ(db.open(),true);
    
    std::string clientID;
    std::string clientID2;
    
    // now create a clientID
    clientID = GAI::ClientID::generateClientID(db);
    
    EXPECT_TRUE( clientID != "" );
    
    // now retrieve it again
    clientID2 = GAI::ClientID::generateClientID(db);
    
    EXPECT_EQ( clientID, clientID2);
    
    // now delete the database and start it back up again
    rc = unlink( test_db.c_str() );    
    db = GAI::DataStoreSqlite(test_db);
    
    // now retrieve it again
    clientID2 = GAI::ClientID::generateClientID(db);
    
    EXPECT_NE( clientID, clientID2);
}

TEST( TimestampTest, create_timestamp )
{
    // create the datastore that client id needs
    const std::string test_db = "test.db";
    int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
    
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_EQ(db.open(),true);
    
    long timestamp1, timestamp2;
    
    // now create a timestamp
    timestamp1 = GAI::Timestamp::generateTimestamp();
    
    // now create a new one
	GAITest::SleepMS(1);
    timestamp2 = GAI::Timestamp::generateTimestamp();
    
    // timestamp should be in realm timestamp1 to 5s
    EXPECT_GT( timestamp2, timestamp1 );
}
