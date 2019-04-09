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
	
    bool success = false;
    event_base* eb = event_base_new();

    GAI::URLConnection connection = GAI::URLConnection( eb );
	connection.setAddress( GAITest::Server::ADDRESS, GAITest::Server::PORT );

    success = false;
    server.clearReceivedRequest();
    connection.request( "/", URLConnectionTestCallback, &success );
  
	event_base_loop( eb, EVLOOP_ONCE ); // send request
	event_base_loop( eb, EVLOOP_ONCE ); // receive request
    EXPECT_TRUE( server.haveReceivedRequest() );
    EXPECT_TRUE( success );
    
    success = true;
    server.clearReceivedRequest();
    connection.request( "/fail", URLConnectionTestCallback, &success );
    
	event_base_loop( eb, EVLOOP_ONCE ); // send request
	event_base_loop( eb, EVLOOP_ONCE ); // receive request
    EXPECT_TRUE( server.haveReceivedRequest() );
    EXPECT_FALSE( success );
    
    event_base_free( eb );
}
