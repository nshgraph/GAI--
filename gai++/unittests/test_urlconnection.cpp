//
//  test_utils.cpp
//  unittests
//
//  Created by Nathan Holmberg on 25/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include <event2/event.h>
#include <event2/http.h>
#include <event2/thread.h>

#include "tinythread.h"
#include "URLConnection.h"
#include "Timestamp.h"


#ifdef WIN32
#include <windows.h>
static void SleepMS(int ms){ Sleep(ms); }
#else
static void SleepMS(int ms){ usleep(ms*1000); }
#endif

class RunServer
{
public:
    RunServer() :
        mbRunning(true),
        mbReceivedRequest(false),
        mThread( )
    {
        mThread = tthread::thread( RunServer::ServerFunction, (void*)this);
    }
    
    ~RunServer()
    {
        mbRunning = false;
        mThread.join();
    }
    
    bool haveReceivedRequest(){
        return mbReceivedRequest;
    }
    void clearReceivedRequest()
    {
        mbReceivedRequest = false;
    }
    
protected:
    static void dump_request_cb(struct evhttp_request *req, void *ctx)
    {
        RunServer* server = (RunServer*)ctx;
        evhttp_send_reply(req, 200, "OK", NULL);
        server->mbReceivedRequest = true;
    }
    static void fail_request_cb(struct evhttp_request *req, void *ctx)
    {
        RunServer* server = (RunServer*)ctx;
        evhttp_send_reply(req, 404, "Not Found", NULL);
        server->mbReceivedRequest = true;
    }
    
    static void ServerFunction( void* ctx )
    {
        RunServer* server = (RunServer*)ctx;
        struct event_base *base;
        struct evhttp *http;
        struct evhttp_bound_socket *handle;
        
        unsigned short port = 8888;
        
        base = event_base_new();
        
        http = evhttp_new(base);
        evhttp_set_cb( http, "/fail", fail_request_cb, ctx );
        evhttp_set_gencb(http, dump_request_cb, ctx);
        
        handle = evhttp_bind_socket_with_handle(http, "127.0.0.1", port);
        
        while( server->mbRunning )
            event_base_loop(base, EVLOOP_NONBLOCK);
        
    }
    bool mbRunning;
    bool mbReceivedRequest;
    tthread::thread mThread;
};


void URLConnectionTestCallback( bool success, void* param )
{
    bool* success_param = (bool*)param;
    *success_param = success;
}


TEST( URLConnection, test_connections )
{
    RunServer server;
    const unsigned long ms_timeout = 1000;
    unsigned long time;
    bool success;
    event_base* eb = event_base_new();
    
    
    SleepMS(1); // allow server time to start running
    server.clearReceivedRequest();
    
    // create a url connection object
    GAI::URLConnection connection = GAI::URLConnection(eb);
    // set the address
    connection.setAddress("127.0.0.1", 8888);
    
    
    success = false;
    server.clearReceivedRequest();
    connection.request( "/", URLConnectionTestCallback, &success );
    
    time = GAI::Timestamp::generateTimestamp();
    while(GAI::Timestamp::generateTimestamp() < time + ms_timeout)
    {
        event_base_loop(eb, EVLOOP_NONBLOCK);
    }
    
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