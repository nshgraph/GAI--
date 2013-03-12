//
//  main.cpp
//  unittests
//
//  Created by Nathan Holmberg on 10/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "DataStoreSqlite.h"

// The fixture for testing class Foo.
class DataStoreSqlLiteTest : public ::testing::Test {
protected:
    
    virtual void SetUp() {
        // delete existing test db
        int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
    }
    
    virtual void TearDown() {
        // delete existing test db
        int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
        
    }
    
    // Objects declared here can be used by all tests in the test case for Foo.
    const std::string test_db = "test.db";
};

TEST_F (DataStoreSqlLiteTest, open_and_close)
{
    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_EQ(db.open(),true);
    // should be open
    EXPECT_EQ(db.isOpen(), true);
    
    db.close();
    // should not be open
    EXPECT_EQ(db.isOpen(), false);
}

TEST_F (DataStoreSqlLiteTest, properties)
{
    const std::string test_key = "test_key";
    const std::string test_value = "test_value";
    // create db
    {
        GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
        EXPECT_EQ(db.open(),true);
        
        // add the property
        EXPECT_EQ(db.addProperty(test_key, test_value),true);
        
        // should have one property
        EXPECT_EQ(db.propertyCount(), 1 );
        
        // fetch the property by name
        {
            std::string ret_value = db.fetchProperty(test_key);
            EXPECT_TRUE( ret_value == test_value );
        }
        // fetch all properties and ensure property is there
        {
            GAI::DataStore::PropertyMap properties = db.fetchProperties();
            GAI::DataStore::PropertyMap::const_iterator it = properties.find(test_key);
            EXPECT_NE( it, properties.end() );
        }
    }
    
    // can create a second reference to the same database
    {
        GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
        EXPECT_EQ(db.open(),true);
        
        // should have one property
        EXPECT_EQ(db.propertyCount(), 1 );
        
        // fetch the property by name
        {
            std::string ret_value = db.fetchProperty(test_key);
            EXPECT_TRUE( ret_value == test_value );
        }
        // fetch all properties and ensure property is there
        {
            GAI::DataStore::PropertyMap properties = db.fetchProperties();
            GAI::DataStore::PropertyMap::const_iterator it = properties.find(test_key);
            EXPECT_NE( it, properties.end() );
        }
        // can delete all properties
        
        EXPECT_TRUE(db.deleteAllProperties());
        
        // should have no properties
        EXPECT_EQ(db.propertyCount(), 0 );
        
        // fetch the property by name should fail
        {
            std::string ret_value = db.fetchProperty(test_key);
            EXPECT_TRUE( ret_value == "" );
        }
        // fetch all properties and ensure property is NOT there
        {
            GAI::DataStore::PropertyMap properties = db.fetchProperties();
            GAI::DataStore::PropertyMap::const_iterator it = properties.find(test_key);
            EXPECT_EQ( it, properties.end() );
        }
        
        
    }
}
