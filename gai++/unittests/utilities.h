
#ifndef __utilities_h__
#define __utilities_h__

#include "tinythread.h"

#include "Hit.h"
#include "Timestamp.h"

struct evhttp_request;

namespace GAITest
{
	void SleepMS( uint32_t ms );

	class Server
	{
	public:
		static const std::string ADDRESS;
		static const int PORT;

		static void thread_func( void* ctx );
		static void callback_200( struct evhttp_request *req, void *ctx );
		static void callback_404( struct evhttp_request *req, void *ctx );

	public:
		Server( const std::string& uri_404 = "/404" );
		~Server();

		bool haveReceivedRequest() const;
		void clearReceivedRequest();

		const std::string mURI404;
		bool mbRunning;
		bool mbReceivedRequest;
		tthread::thread mThread;
	};

	class TestHit : public GAI::Hit
	{
	public:
		TestHit() : Hit() { mTimestamp = GAI::Timestamp::generateTimestamp(); }
		TestHit( const std::string& version, const std::string& url, const uint64_t timestamp ) : Hit( -1, version, url, timestamp ) {}
		TestHit( const Hit& hit ) : Hit( hit.getId(), hit.getGaiVersion(), hit.getDispatchURL(), hit.getTimestamp() ) {}

		bool operator== (const TestHit& o ) const
		{
			return o.getDispatchURL() == getDispatchURL() &&
				   o.getGaiVersion() == getGaiVersion() &&
				   o.getTimestamp() == getTimestamp();
		}
	};
}

#endif
