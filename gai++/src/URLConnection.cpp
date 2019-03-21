
#include "URLConnection.h"

#include <sstream>

#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>

#include "Platform.h"
#include "DebugPrint.h"

namespace GAI
{
    struct ConnectionCallbackData
    ///
    /// This struct is used to internally hold data for a request's callback
    ///
    {
        URLConnection::URLConnectionCompleteCB callback;
        void* data;
    };
    
    void url_connection_callback(evhttp_request *req, void *arg)
    ///
    /// Internal Callback Function used by URLConnection
    ///
    /// @param req
    ///     The request that this callback is responding too (note, NULL means the request timed out)
    ///
    /// @param arg
    ///     The user supplied data for this request
    ///
    {
        ConnectionCallbackData* cb_data = (ConnectionCallbackData*)arg;
        if( req && req->response_code == 200 )
        {
			DEBUG_PRINT( "Connection Callback: 200 OK"  << std::endl );
            cb_data->callback(true, cb_data->data);
        }
        else
        {
			DEBUG_PRINT( "Connection Callback: " << (req ? req->response_code : 0) << " Error" << std::endl );
            cb_data->callback(false, cb_data->data);
        }
        
        delete cb_data;
    }
    
    URLConnection::URLConnection( event_base *base ) :
    mEventBase(base),
	mConnection(NULL)
    ///
    /// Constructor
    ///
    /// @param base
    ///     The event_base to be used by the URL connection. Allows the connection to run asynchronously.
    ///
    {
    }

	URLConnection::~URLConnection()
	{
		if( mConnection )
		{
			evhttp_connection_free( mConnection );
		}
	}

    void URLConnection::request( const std::string& url, URLConnectionCompleteCB callback, void* callback_data )
    ///
    /// Function to request a page via the GET protocol
    ///
    /// @param url
    ///     The url to request (note, this does NOT include the host)
    ///
    /// @param callback
    ///     Callback to call on success / failure of the request
    ///
    /// @param callback_data
    ///     Data to send back with callback
    ///
    {
        // create a struct for handling the real callback for the user of URLConnection
        // should be deallocated in the url_connection_callback function
        ConnectionCallbackData* cb_data = new ConnectionCallbackData();
        cb_data->callback = callback;
        cb_data->data = callback_data;
        
        evhttp_request* request = evhttp_request_new(url_connection_callback, cb_data);
        
        // set up appropriate headers for a GET request
        char* host_address;
        ev_uint16_t port;
        evhttp_connection_get_peer(mConnection, &host_address, &port);
        evhttp_add_header( request->output_headers, "Host", host_address );
        evhttp_add_header( request->output_headers, "User-Agent", mUserAgent.c_str() );		
        evhttp_make_request(mConnection, request, EVHTTP_REQ_GET, url.c_str());
        
    }
    
    void URLConnection::requestPOST(const std::string& url, const std::string& payload, URLConnectionCompleteCB callback, void* callback_data )
	///
    /// Function to request a page via the POST protocol
    ///
    /// @param url
    ///     The url to request (note, this does NOT include the host)
    ///
    /// @param payload
    ///     POST payload to send
    ///
    /// @param callback
    ///     Callback to call on success / failure of the request
    ///
    /// @param callback_data
    ///     Data to send back with callback
    ///
    {
        // create a struct for handling the real callback for the user of URLConnection
        // should be deallocated in the url_connection_callback function
        ConnectionCallbackData* cb_data = new ConnectionCallbackData();
        cb_data->callback = callback;
        cb_data->data = callback_data;
        
        evhttp_request* request = evhttp_request_new(url_connection_callback, cb_data);
        
        // set up appropriate headers for a GET request
        char* host_address;
        ev_uint16_t port;
        evhttp_connection_get_peer(mConnection, &host_address, &port);
        evhttp_add_header( request->output_headers, "Host", host_address );
		evhttp_add_header( request->output_headers, "User-Agent", mUserAgent.c_str() );
        evhttp_add_header( request->output_headers, "Connection", "keep-alive" );
        evhttp_add_header( request->output_headers, "Content-Type", "application/x-www-form-urlencoded; charset=UTF-8" );
        evbuffer_add(request->output_buffer, payload.c_str(), payload.size());
		
        evhttp_make_request(mConnection, request, EVHTTP_REQ_POST, url.c_str());
    }
    
    void URLConnection::setAddress( const std::string& address, int port )
    ///
    /// Set the host address to be used for this request
    ///
    /// @param address
    ///     Host Address
    ///
    /// @param port
    ///     Host Port
    ///
    {
		if( mConnection )
		{
			evhttp_connection_free( mConnection );
		}
        mConnection = evhttp_connection_base_new(mEventBase, NULL, address.c_str(), port);
    }
    
    void URLConnection::createUserAgentString( const std::string& product, const std::string& version )
    ///
    /// Generate the user agent string to be sen with every request
    ///
    /// @param product
    ///     Host Address
    ///
    /// @param version
    ///     Host Port
    ///
    {
        std::ostringstream ss;
		ss << product << "/" << version << Platform::GetPlatformUserAgentString();
		mUserAgent = ss.str();
    }
	
	std::string URLConnection::getUserAgentString()
	///
	/// Get the user agent string
	///
	/// @return
	/// The current user agent string
	///
	{
		return mUserAgent;
	}
}
