
#include "URLConnection.h"

#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>


namespace GAI
{
    struct ConnectionCallbackData
    {
        URLConnection::URLConnectionCompleteCB callback;
        void* data;
    };
    
    void url_connection_callback(evhttp_request *req, void *arg)
    {
        ConnectionCallbackData* cb_data = (ConnectionCallbackData*)arg;
        if( req && req->response_code == 200 )
            cb_data->callback(true, cb_data->data);
        else
            cb_data->callback(false, cb_data->data);
        
        delete cb_data;
    }
    
    URLConnection::URLConnection( event_base *base ) :
    mEventBase(base)
    {
    }
    void URLConnection::request( const std::string& url, URLConnectionCompleteCB callback, void* callback_data )
    {
        // create a struct for handling the real callback for the user of URLConnection
        // should be deallocated in the url_connection_callback function
        ConnectionCallbackData* cb_data = new ConnectionCallbackData();
        cb_data->callback = callback;
        cb_data->data = callback_data;
        
        evhttp_request* request = evhttp_request_new(url_connection_callback, cb_data);
        
        char* host_address;
        ev_uint16_t port;
        evhttp_connection_get_peer(mConnection, &host_address, &port);
        evhttp_add_header( request->output_headers, "Host", host_address );
        evhttp_add_header( request->output_headers, "User-Agent", "GAI++ Mac OSX 10.8" );

        evhttp_make_request(mConnection, request, EVHTTP_REQ_GET, url.c_str());
        
    }
    
    void URLConnection::setAddress( const std::string& address, int port )
    {
        mConnection = evhttp_connection_base_new(mEventBase,NULL,address.c_str(), port);
    }
}
