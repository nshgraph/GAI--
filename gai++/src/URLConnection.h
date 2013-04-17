
#ifndef __URLConnection_h__
#define __URLConnection_h__

#include <string>

struct evhttp_connection;
struct event_base;

namespace GAI
{
	class URLConnection
	{
    public:
        typedef void (*URLConnectionCompleteCB)(bool success, void* callback_data);
    public:
        URLConnection( event_base *base );
        void request(const std::string& url, URLConnectionCompleteCB callback, void* callback_data );
        void setAddress( const std::string& address, int port );
        void createUserAgentString( const std::string& product, const std::string& version );
    private:
        evhttp_connection *mConnection; //< Connection object to send all requests from
        event_base *mEventBase; //<Event Base for async requests
        
        std::string mUserAgent;
	};
}

#endif
