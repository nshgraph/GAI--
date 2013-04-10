//
//  main.cpp
//  unittests
//
//  Created by Nathan Holmberg on 10/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "DataStoreSqlite.h"
#include "Hit.h"

class HitTestClass : public GAI::Hit
{
public:
    HitTestClass(){}
    HitTestClass( const std::string url, const double timestamp ) : Hit("1",url,timestamp)
    {
    }
    HitTestClass( const Hit& hit ) : Hit(hit.getGaiVersion(), hit.getDispatchURL(), hit.getTimestamp())
    {
    }
    
    bool operator == (const HitTestClass& o ) const
    {
        return o.getDispatchURL() == getDispatchURL() &&
        o.getGaiVersion() == getGaiVersion() &&
        o.getTimestamp() == getTimestamp();
    }
};

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

TEST_F (DataStoreSqlLiteTest, properties_kv)
{
    const std::string test_key = "test_key";
    const std::string test_value = "test_value";
    const std::string test_value2 = "test_value2";
    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_EQ(db.open(),true);
    
    // add the property
    EXPECT_EQ(db.addProperty(test_key, test_value),true);
    
    // should have one property
    EXPECT_EQ(db.propertyCount(), 1 );
    
    // change the value of a property
    EXPECT_EQ(db.addProperty(test_key, test_value2),true);
    
    // should still have one property
    
    // should have one property
    EXPECT_EQ(db.propertyCount(), 1 );
    
    // fetch the property by name
    {
        std::string ret_value = db.fetchProperty(test_key);
        // check that it has the *new* value
        EXPECT_TRUE( ret_value == test_value2 );
    }

}

TEST_F (DataStoreSqlLiteTest, hits)
{
    // Create a test hit
    const std::string test_url = "test_url_string";
    const double test_timestamp = 15.0;
    std::list<GAI::Hit> results;
    std::list<GAI::Hit> hits_to_add;
    HitTestClass result_hit;
    HitTestClass test_hit = HitTestClass( test_url, test_timestamp );
    
    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_TRUE(db.open());
    
    // Store the test hit
    EXPECT_TRUE(db.addHit(test_hit));
    
    //should have one hit
    EXPECT_EQ(db.hitCount(),1);
    
    // Fetch the hit (don't clear)
    results = db.fetchHits(1, false);
    
    //should still have one hit
    EXPECT_EQ(db.hitCount(),1);
    
    //check that the size is one
    EXPECT_EQ(results.size(),1);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    result_hit = HitTestClass( results.front()) ;
    EXPECT_EQ( test_hit, result_hit);
    
    // now fetch with clear
    results = db.fetchHits(1, true);
    
    //should no longer have any hits
    EXPECT_EQ(db.hitCount(),0);
    
    //check that the size is one
    ASSERT_EQ(results.size(),1);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    EXPECT_EQ( test_hit, HitTestClass( results.front()) );
    
    // check that we can add multiple hits
    hits_to_add.push_back(test_hit);
    hits_to_add.push_back(test_hit);
    EXPECT_TRUE(db.addHits(hits_to_add));
    
    EXPECT_EQ(db.hitCount(),2);
    
}

TEST_F (DataStoreSqlLiteTest, hits2)
{
    // Create a test hit
    const std::string test_url = "test_url_string";
    const double test_timestamp = 15.0;
    std::list<GAI::Hit> results;
    HitTestClass result_hit;
    HitTestClass test_hit = HitTestClass( test_url, test_timestamp );
    
    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_TRUE(db.open());
    
    // Store the test hit
    EXPECT_TRUE(db.addHit(test_hit));
    // Store a second hit
    EXPECT_TRUE(db.addHit(test_hit));
    
    //should have one hit
    EXPECT_EQ(db.hitCount(),2);
    
    // Fetch the hit (don't clear)
    results = db.fetchHits(1, false);
    
    //should still have one hit
    EXPECT_EQ(db.hitCount(),2);
    
    //check that the size is one
    EXPECT_EQ(results.size(),1);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    result_hit = HitTestClass( results.front()) ;
    EXPECT_EQ( test_hit, result_hit);
    
    // now fetch with clear
    results = db.fetchHits(1, true);
    
    //should still have one hit
    EXPECT_EQ(db.hitCount(),1);
    
    //check that the size is one
    ASSERT_EQ(results.size(),1);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    EXPECT_EQ( test_hit, HitTestClass( results.front()) );
}
