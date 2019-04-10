#include "gtest/gtest.h"
#include "utilities.h"

#include "Dispatcher.h"
#include "DataStoreSqlite.h"

namespace
{
	// The Dispatcher queue only processes an event once
	// every 2000ms, we must allow for this in some tests
	// with some extra margin for other delays
	const int DISPATCHER_TIMEOUT = 3000;
}

class DispatcherTestClass : public GAI::Dispatcher
{
public:
	DispatcherTestClass( GAI::DataStore& data_store )
		: Dispatcher( data_store, false, 1, GAITest::Server::ADDRESS, GAITest::Server::PORT )
		, mbDispatchComplete( false )
	{
	}

	virtual void dispatch()
	{
		Dispatcher::dispatch();
		mbDispatchComplete = true;
	}

	bool mbDispatchComplete;
};

class DispatcherTest : public ::testing::Test
{
protected:
	DispatcherTest() : test_db("test.db") {}

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

TEST_F( DispatcherTest, hits_are_removed_on_successful_response )
{
	GAITest::Server server;

	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );

	GAITest::TestHit hit;
	dispatcher.storeHit( hit );
	EXPECT_EQ( 1, data_store.entityCount() );

	dispatcher.startEventLoop();
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for thread loop
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for hit dispatch
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for post process
	EXPECT_EQ( 0, data_store.entityCount() );
}

TEST_F( DispatcherTest, hits_are_not_removed_on_failed_response )
{
	GAITest::Server server( "/collect" );

	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );

	GAITest::TestHit hit;
	dispatcher.storeHit( hit );
	EXPECT_EQ( 1, data_store.entityCount() );

	dispatcher.startEventLoop();
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for thread loop
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for hit dispatch
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for post process
	EXPECT_EQ( 1, data_store.entityCount() );
}

TEST_F( DispatcherTest, hits_are_not_removed_on_no_response )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );

	GAITest::TestHit hit;
	dispatcher.storeHit( hit );
	EXPECT_EQ( 1, data_store.entityCount() );

	dispatcher.startEventLoop();
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for thread loop
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for hit dispatch
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for post process
	EXPECT_EQ( 1, data_store.entityCount() );
}

TEST_F( DispatcherTest, dispatch_interval )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );

	dispatcher.startEventLoop();
	dispatcher.setDispatchInterval( 4 );

	GAITest::SleepMS( 4000 );
	EXPECT_FALSE( dispatcher.mbDispatchComplete );

	GAITest::SleepMS( DISPATCHER_TIMEOUT );
	EXPECT_TRUE( dispatcher.mbDispatchComplete );
}

TEST_F( DispatcherTest, dispatch_immediately )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );

	dispatcher.queueDispatch();
	GAITest::SleepMS( DISPATCHER_TIMEOUT );
	EXPECT_TRUE( dispatcher.mbDispatchComplete );
}

TEST_F( DispatcherTest, dispatch_multiple )
{
	GAITest::Server server;

	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );
	dispatcher.startEventLoop();

	// Dispatch 1
	dispatcher.storeHit( GAITest::TestHit() );
	EXPECT_EQ( 1, data_store.entityCount() );
	dispatcher.queueDispatch();
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for hit dispatch
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for post process
	EXPECT_EQ( 0, data_store.entityCount() );
	
	// Dispatch 2
	dispatcher.storeHit( GAITest::TestHit() );
	EXPECT_EQ( 1, data_store.entityCount() );
	dispatcher.queueDispatch();
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for hit dispatch
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for post process
	EXPECT_EQ( 0, data_store.entityCount() );
	
	// Dispatch 3
	dispatcher.storeHit( GAITest::TestHit() );
	EXPECT_EQ( 1, data_store.entityCount() );
	dispatcher.queueDispatch();
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for hit dispatch
	GAITest::SleepMS( DISPATCHER_TIMEOUT ); // Wait for post process
	EXPECT_EQ( 0, data_store.entityCount() );
}

TEST_F( DispatcherTest, opt_out )
{
	GAI::DataStoreSqlite data_store = GAI::DataStoreSqlite( test_db );
	DispatcherTestClass dispatcher = DispatcherTestClass( data_store );
	dispatcher.setOptOut( true );

	GAITest::TestHit hit;
	dispatcher.storeHit( hit );
	EXPECT_EQ( 0, data_store.hitCount() );
}
