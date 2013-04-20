//
//  test_dispatcher.cpp
//  unittests
//
//  Created by Steve Hosking on 10/03/13.
//  Copyright (c) 2013 hoseking. All rights reserved.
//

#include "gtest/gtest.h"

#include "GAIDefines.h"

#include "Dispatcher.h"
#include "DataStoreSqlite.h"

#include <iostream>
#include <string>

class DispatcherTestClass : public GAI::Dispatcher
{
public:
	DispatcherTestClass( GAI::DataStore& data_store, bool opt_out, double dispatch_interval ) :
	Dispatcher( data_store, opt_out, dispatch_interval ),
	mbCallbackComplete( false )
	{
	}
	
	virtual void dispatch()
	{
		mbCallbackComplete = true;
	}
	
	int getHitCount() const
	{
		return mDataStore.entityCount();
	}
	
	bool mbCallbackComplete;
};

class HitTestClass : public GAI::Hit
{
public:
    HitTestClass() : Hit(){}
	
};

class DispatcherTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
        // delete existing test db
        int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
    }
    
    virtual void TearDown()
	{
        // delete existing test db
        int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
        
    }
    
    // Objects declared here can be used by all tests in the test case.
    const std::string test_db = "test.db";
};

TEST_F( DispatcherTest, dispatch_interval_callback )
{
	const double dispatch_interval = 0.1;
	
    GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store, false, dispatch_interval );
	
	usleep( dispatch_interval * 1000000 * 1.5 );
	
	EXPECT_TRUE( dispatcher.mbCallbackComplete );
}

TEST_F( DispatcherTest, set_dispatch_interval )
{
	const double dispatch_interval_1 = 1000;
	const double dispatch_interval_2 = 0.05;
	
    GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store, false, dispatch_interval_1 );
	dispatcher.setDispatchInterval( dispatch_interval_2 );
	
	usleep( dispatch_interval_2 * 1000000 * 1.5 );
	
	EXPECT_TRUE( dispatcher.mbCallbackComplete );
}

TEST_F( DispatcherTest, opt_out )
{
	const bool opt_out = true;
	
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store, opt_out, kDispatchInterval );
	
	HitTestClass hit;
	dispatcher.storeHit( hit );
	
	EXPECT_EQ( dispatcher.getHitCount(), 0 );
}

TEST_F( DispatcherTest, store_hit )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store, false, kDispatchInterval );
	
	HitTestClass hit;
	dispatcher.storeHit( hit );
	
	EXPECT_EQ( dispatcher.getHitCount(), 1 );
}

TEST_F( DispatcherTest, dispatch )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
    GAI::Dispatcher dispatcher = GAI::Dispatcher( data_store, false, 2 );
    std::map<std::string, std::string> parameters;
    parameters[kAppNameModelKey] = "app";
	
	HitTestClass hit;
    hit.setParameters( parameters );
	dispatcher.storeHit( hit );
	usleep( 2 * 1000000 * 1.5 );
    
    // should have printed
}


TEST_F( DispatcherTest, immediate_dispatch )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
    DispatcherTestClass dispatcher = DispatcherTestClass( data_store, false, 200 );
    std::map<std::string, std::string> parameters;
    parameters[kAppNameModelKey] = "app";
	
    dispatcher.queueDispatch();
	usleep( 2 * 1000000 * 1.5 );
	EXPECT_TRUE( dispatcher.mbCallbackComplete );
}

TEST_F( DispatcherTest, cancel_dispatch )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
    GAI::Dispatcher dispatcher = GAI::Dispatcher( data_store, false, 200 );
    std::map<std::string, std::string> parameters;
    parameters[kAppNameModelKey] = "app";
	
    for( int i=0;i<1000;i++)
    {
        HitTestClass hit;
        hit.setParameters( parameters );
        dispatcher.storeHit( hit );
    }
    EXPECT_EQ( data_store.hitCount(), 1000 );
    dispatcher.queueDispatch();
    
	usleep( 1000 );
    
    EXPECT_LT( data_store.hitCount(), 1000 );
    dispatcher.cancelDispatch();
    EXPECT_LT( data_store.hitCount(), 1000 );
    int before_count = data_store.hitCount();
	usleep( 5000 );
    // note: dispatch cancel can allow up to kDispatchBlockSize
    EXPECT_GE( data_store.hitCount(), before_count - kDispatchBlockSize);
    
}




TEST_F( DispatcherTest, options )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
    GAI::Dispatcher dispatcher = GAI::Dispatcher( data_store, false, 2 );
    // can get and set https
    dispatcher.setUseHttps(false);
    EXPECT_EQ( dispatcher.isUseHttps(), false );
    dispatcher.setUseHttps(true);
    EXPECT_EQ( dispatcher.isUseHttps(), true );
}

