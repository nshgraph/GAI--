
#include "URLConnection.h"

#include <event2/http.h>


namespace GAI
{
    void url_connection_callback(struct evhttp_request *req, void *arg)
    {
        printf("url_conn_back");
    }
    
    URLConnection::URLConnection( event_base *base ) :
    mEventBase(base)
    {
    }
    void URLConnection::request( const std::string& url )
    {
        
        evhttp_request* request = evhttp_request_new(url_connection_callback, NULL);
        evhttp_make_request(mConnection, request, EVHTTP_REQ_GET, url.c_str());
        
    }
    
    void URLConnection::setAddress( const std::string& address, int port )
    {
        mConnection = evhttp_connection_base_new(mEventBase,NULL,address.c_str(), port);
    }
}
