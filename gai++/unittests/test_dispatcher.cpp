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
	
	virtual void queueDispatch()
	{
		mbCallbackComplete = true;
	}


	bool mbCallbackComplete;
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
	const int dispatch_interval = 1;
	
    GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store, false, dispatch_interval );
	
	sleep( dispatch_interval + 1 );
	
	EXPECT_TRUE( dispatcher.mbCallbackComplete );
}

TEST_F( DispatcherTest, set_dispatch_interval )
{
	const int dispatch_interval_1 = 1000;
	const int dispatch_interval_2 = 1;
	
    GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store, false, dispatch_interval_1 );
	dispatcher.setDispatchInterval( dispatch_interval_2 );
	
	sleep( dispatch_interval_2 + 1 );
	
	EXPECT_TRUE( dispatcher.mbCallbackComplete );
}
