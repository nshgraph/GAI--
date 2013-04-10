
#include "Hit.h"

#include <string>
#include <map>

#include <evhttp.h>

#include "GAIDefines.h"

#include "HitBuilder.h"

namespace GAI
{
    
    Hit::Hit() :
    mGaiVersion( kGAIVersion ),
	mDispatchURL( "" ),
	mTimestamp( 0 )
    ///
    /// Default Constructor
    /// Uses the defined version of Google Analytics
    ///
    {
    }
    
    Hit::Hit( const std::string& version, const std::string& url, const double timestamp ) :
    mGaiVersion( version ),
	mDispatchURL( url ),
	mTimestamp( timestamp )
    ///
    /// Constructor
    /// This should be used when deserializing a pre-made hit
    ///
    /// @param version
    ///     The Google Analytics protocol version
    /// @param url
    ///     The ready-encoded set of parameters
    /// @param timestamp
    ///     The timestamp of this hit
    ///
    {
    }
    
    void Hit::setParameters( const std::map<std::string, std::string> parameters )
    ///
    /// Sets the parameters to be used with this Hit. Will clear any pre-existing parameters
    ///
    /// @param parameters
    ///     The set of parameters to be used by this Hit. 
    ///
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
    ///
    /// Retrieves the url of encoded values for this hit
    ///
    /// @return
    ///     The encoded dispatch url
    ///
	{
        return mDispatchURL;
    }
    
    std::string Hit::getGaiVersion() const
    ///
    /// Retrieves the Google Analytics version of this hit
    ///
    /// @return
    ///     The GAI version
    ///
	{
        return mGaiVersion;
    }
    
    void Hit::setTimestamp( double timestamp )
    ///
    /// Sets the timestamp of this hi
    ///
    /// @param timestamp
    ///     The timestamp of this hit
    ///
	{
        mTimestamp = timestamp;
    }
    
    double Hit::getTimestamp() const
    ///
    /// Retrieve the timestamp of this hit
    ///
    /// @return
    ///     The timestamp of this hit
    ///
	{
        return mTimestamp;
    }
    
}
