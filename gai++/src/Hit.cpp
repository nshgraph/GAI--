#include <string>
#include <map>


#include "Hit.h"
#include "RequestBuilder.h"
#include "HitBuilder.h"

namespace GAI
{
    
    Hit::Hit()
    {
        mGaiVersion = "1";
        mDispatchURL = "";
        mTimestamp = 0;
    }
    
    void Hit::setParameters(std::map<std::string, std::string> aParameters) {
        // generate dispatch url from parameters
        std::string url = "";
        bool first_param = true;
        for( std::map<std::string, std::string>::const_iterator it = aParameters.begin(), it_end = aParameters.end(); it != it_end; it++)
        {
            if( !first_param )
                url += "&";
            url+=it->first;
            url+="=";
            url+= it->second;
            first_param = false;
        }
        mDispatchURL = url;
    }
    
    std::string Hit::getDispatchURL() const{
        std::string full_dispatch_url = mDispatchURL;
        // append the version
        if( full_dispatch_url != "" )
            full_dispatch_url += "&";
        full_dispatch_url+="v="+mGaiVersion;
        // TODO: URL ENCODING
        return full_dispatch_url;
    }
    
    std::string Hit::getGaiVersion() const{
        return this->mGaiVersion;
    }
    
    void Hit::setTimestamp(double aTimestamp) {
        this->mTimestamp = aTimestamp;
    }
    
    double Hit::getTimestamp() const{
        return this->mTimestamp;
    }
    
}