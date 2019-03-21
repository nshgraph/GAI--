
#include "utilities.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/thread.h>

using namespace GAITest;

void GAITest::SleepMS( uint32_t ms )
{
#ifdef WIN32
	Sleep( ms );
#else
	usleep( ms * 1000 );
#endif
}

const std::string Server::ADDRESS = "127.0.0.1";
const int Server::PORT = 8081;

Server::Server( const std::string& uri_404 )
: mURI404( uri_404 )
, mbRunning( true )
, mbReceivedRequest( false )
, mThread( Server::thread_func, (void*)this )
{
}

Server::~Server()
{
	mbRunning = false;
	mThread.join();
}

bool Server::haveReceivedRequest() const
{
	return mbReceivedRequest;
}

void Server::clearReceivedRequest()
{
	mbReceivedRequest = false;
}

void Server::thread_func( void* ctx )
{
	Server* server = (Server*)ctx;
	struct event_base* base;
	struct evhttp* http;
	struct evhttp_bound_socket* handle;

	base = event_base_new();
	http = evhttp_new( base );
	evhttp_set_cb( http, server->mURI404.c_str(), callback_404, ctx );
	evhttp_set_gencb( http, callback_200, ctx );
	handle = evhttp_bind_socket_with_handle( http, ADDRESS.c_str(), PORT );

	while( server->mbRunning )
	{
		event_base_loop( base, EVLOOP_NONBLOCK );
		SleepMS( 5 );
	}

	evhttp_free( http );
	event_base_free( base );
}

void Server::callback_200( struct evhttp_request *req, void *ctx )
{
	evhttp_send_reply( req, 200, "OK", NULL );

	Server* server = (Server*)ctx;
	server->mbReceivedRequest = true;
}

void Server::callback_404( struct evhttp_request *req, void *ctx )
{
	evhttp_send_reply( req, 404, "Not Found", NULL );

	Server* server = (Server*)ctx;
	server->mbReceivedRequest = true;
}
