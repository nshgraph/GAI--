//
//  test_clientid.cpp
//  unittests
//
//  Created by Nathan Holmberg on 25/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "ClientID.h"
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
