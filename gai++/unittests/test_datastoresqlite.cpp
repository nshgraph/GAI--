//
//  main.cpp
//  unittests
//
//  Created by Nathan Holmberg on 10/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"
#include "utilities.h"

#include "DataStoreSqlite.h"

class DataStoreSqlLiteTest : public ::testing::Test
{
protected:
	DataStoreSqlLiteTest() : test_db("test.db"){}

	virtual void SetUp()
	{
        unlink( test_db.c_str() );
    }
    
    virtual void TearDown()
	{
		unlink( test_db.c_str() );
	}
    
    const std::string test_db;
};

TEST_F( DataStoreSqlLiteTest, open_and_close )
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

TEST_F( DataStoreSqlLiteTest, properties )
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

TEST_F( DataStoreSqlLiteTest, properties_kv )
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

TEST_F( DataStoreSqlLiteTest, hits )
{
    // Create a test hit
    const std::string test_url = "test_url_string";
    const double test_timestamp = 15.0;
    std::list<GAI::Hit> results;
    std::list<GAI::Hit> hits_to_add;
    GAITest::TestHit result_hit;
    GAITest::TestHit test_hit = GAITest::TestHit( "1", test_url, test_timestamp );
    
    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_TRUE(db.open());
    
    // Store the test hit
    EXPECT_TRUE(db.addHit(test_hit));
    
    //should have one hit
    EXPECT_EQ(db.hitCount(), 1);
    
    // Fetch the hit (don't clear)
    results = db.fetchHits(0, 1);
    
    //should still have one hit
    EXPECT_EQ(db.hitCount(), 1);
    
    //check that the size is one
    EXPECT_EQ(results.size(), 1);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    result_hit = GAITest::TestHit( results.front() );
    EXPECT_EQ( test_hit, result_hit);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    EXPECT_EQ( test_hit, GAITest::TestHit( results.front()) );
    
    // check that we can add multiple hits
    hits_to_add.push_back(test_hit);
    hits_to_add.push_back(test_hit);
    EXPECT_TRUE(db.addHits(hits_to_add));
    
    EXPECT_EQ(db.hitCount(), 3);
}

TEST_F( DataStoreSqlLiteTest, hits2 )
{
    // Create a test hit
    const std::string test_url = "test_url_string";
    const double test_timestamp = 15.0;
    std::list<GAI::Hit> results;

	GAITest::TestHit test_hit_one = GAITest::TestHit( "1", test_url, test_timestamp );
	GAITest::TestHit test_hit_two = GAITest::TestHit( "2", test_url, test_timestamp );

    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_TRUE(db.open());
    
    // store the test hit
    EXPECT_TRUE(db.addHit(test_hit_one));

    // store a second hit
    EXPECT_TRUE(db.addHit(test_hit_two));
    
    // should have two hits
    EXPECT_EQ(db.hitCount(), 2);
    
    // fetch the first hit
    results = db.fetchHits(0, 1);
    
    // should still have two hits
    EXPECT_EQ(db.hitCount(), 2);
    
    // check that the size is one
    EXPECT_EQ(results.size(), 1);
    
    // check that the hit that is there is fundamentally equivalent to that tested
    GAITest::TestHit result_hit = GAITest::TestHit( results.front() );
    EXPECT_EQ(test_hit_one, result_hit);
}

TEST_F( DataStoreSqlLiteTest, fetch_hits_timestamp )
{
	std::list<GAI::Hit> results;
	GAITest::TestHit test_hit_one = GAITest::TestHit( "1", "", 10 );
	GAITest::TestHit test_hit_two = GAITest::TestHit( "2", "", 20 );

	GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
	db.open();
	db.addHit( test_hit_one );
	db.addHit( test_hit_two );

	results = db.fetchHits(0, 10);
	EXPECT_EQ( 2, results.size() );
	EXPECT_EQ( "1", results.front().getGaiVersion() );
	results.pop_front();
	EXPECT_EQ( "2", results.front().getGaiVersion() );

	results = db.fetchHits(10, 10);
	EXPECT_EQ( 1, results.size() );
	EXPECT_EQ( "2", results.front().getGaiVersion() );

	results = db.fetchHits(20, 10);
	EXPECT_EQ( 0, results.size() );
}

TEST_F( DataStoreSqlLiteTest, fetch_hits_limit )
{
	std::list<GAI::Hit> results;
	GAITest::TestHit test_hit_one = GAITest::TestHit( "1", "", 1 );
	GAITest::TestHit test_hit_two = GAITest::TestHit( "2", "", 1 );

	GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
	db.open();
	db.addHit( test_hit_one );
	db.addHit( test_hit_two );

	results = db.fetchHits(0, 1);
	EXPECT_EQ( 1, results.size() );

	results = db.fetchHits(0, 2);
	EXPECT_EQ( 2, results.size() );

	results = db.fetchHits(0, 10);
	EXPECT_EQ( 2, results.size() );
}

TEST_F( DataStoreSqlLiteTest, delete_hit )
{
	std::list<GAI::Hit> results;
	GAITest::TestHit test_hit_one = GAITest::TestHit( "1", "", 1 );
	GAITest::TestHit test_hit_two = GAITest::TestHit( "2", "", 1 );

	GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
	db.open();
	db.addHit( test_hit_one );
	db.addHit( test_hit_two );
	EXPECT_EQ( 2, db.hitCount() );

	results = db.fetchHits(0, 1);
	EXPECT_TRUE( db.deleteHit( results.front().getId() ) );
	EXPECT_EQ( 1, db.hitCount() );
}

TEST_F( DataStoreSqlLiteTest, delete_all_hits )
{
	std::list<GAI::Hit> results;
	GAITest::TestHit test_hit_one = GAITest::TestHit( "1", "", 1 );
	GAITest::TestHit test_hit_two = GAITest::TestHit( "2", "", 1 );

	GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
	db.open();
	db.addHit( test_hit_one );
	db.addHit( test_hit_two );

	EXPECT_EQ( 2, db.hitCount() );
	EXPECT_TRUE( db.deleteAllHits() );
	EXPECT_EQ( 0, db.hitCount() );
}
