
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
    private:
        evhttp_connection *mConnection;
        event_base *mEventBase;
	};
}

#endif
