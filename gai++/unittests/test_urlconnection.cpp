//
//  test_utils.cpp
//  unittests
//
//  Created by Nathan Holmberg on 25/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"
#include "utilities.h"

#include <event2/event.h>

#include "URLConnection.h"
#include "Timestamp.h"

void URLConnectionTestCallback( bool success, void* param )
{
    bool* success_param = (bool*)param;
    *success_param = success;
}

TEST( URLConnection, test_connections )
{
	GAITest::Server server( "/fail" );
	
    const unsigned long ms_timeout = 1000;
    unsigned long time;
    bool success;
    event_base* eb = event_base_new();

    GAI::URLConnection connection = GAI::URLConnection( eb );
	connection.setAddress( GAITest::Server::ADDRESS, GAITest::Server::PORT );

    success = false;
    server.clearReceivedRequest();
    connection.request( "/", URLConnectionTestCallback, &success );
    
    time = GAI::Timestamp::generateTimestamp();
    while(GAI::Timestamp::generateTimestamp() < time + ms_timeout)
    {
        event_base_loop(eb, EVLOOP_NONBLOCK);
    }
    
    GAITest::SleepMS(5); // allow server time to start running
    
    EXPECT_TRUE( server.haveReceivedRequest() );
    EXPECT_TRUE( success );
    
    success = false;
    server.clearReceivedRequest();
    connection.request( "/fail", URLConnectionTestCallback, &success );
    
    time = GAI::Timestamp::generateTimestamp();
    while(GAI::Timestamp::generateTimestamp() < time + ms_timeout)
    {
        event_base_loop(eb, EVLOOP_NONBLOCK);
    }
    
    EXPECT_TRUE( server.haveReceivedRequest() );
    EXPECT_FALSE( success );
    
    // destroy event loop
    event_base_free( eb );
}
