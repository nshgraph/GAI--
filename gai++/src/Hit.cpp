
#include "Hit.h"

#include <string>
#include <map>

#include <evhttp.h>

#include "RequestBuilder.h"
#include "HitBuilder.h"

namespace GAI
{
    
    Hit::Hit() :
    mGaiVersion( kGAIVersion ),
	mDispatchURL( "" ),
	mTimestamp( 0 )
    {
    }
    
    Hit::Hit( const std::string& version, const std::string& url, const double timestamp ) :
    mGaiVersion( version ),
	mDispatchURL( url ),
	mTimestamp( timestamp )
    {
    }
    
    void Hit::setParameters( const std::map<std::string, std::string> parameters )
	{
        // generate dispatch url from parameters
        std::string url = "";
        bool first_param = true;
        for( std::map<std::string, std::string>::const_iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; it++)
        {
            if( !first_param )
                url += "&";
			
			url += it->first;
            url += "=";
			url += evhttp_encode_uri( it->second.c_str() ); //encode the user entered parameters
			
            first_param = false;
        }
		
        // append the version
        url += "&v=" + mGaiVersion;
        
		// set the url
        mDispatchURL = url;
    }
    
    std::string Hit::getDispatchURL() const
	{
        return mDispatchURL;
    }
    
    std::string Hit::getGaiVersion() const
	{
        return mGaiVersion;
    }
    
    void Hit::setTimestamp( double timestamp )
	{
        mTimestamp = timestamp;
    }
    
    double Hit::getTimestamp() const
	{
        return mTimestamp;
    }
    
}
