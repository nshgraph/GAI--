#include <string>
#include <map>


#include "Hit.h"
#include "RequestBuilder.h"
#include "HitBuilder.h"

#define GAI_VERSION "1"

namespace GAI
{
    
    Hit::Hit() :
    mGaiVersion(GAI_VERSION)
    {
        mDispatchURL = "";
        mTimestamp = 0;
    }
    
    Hit::Hit(const std::string& version, const std::string& url, const double timestamp) :
    mGaiVersion(version)
    {
        mDispatchURL = url;
        mTimestamp = timestamp;
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
        // append the version
        if( url != "" )
            url += "&";
        url+="v="+mGaiVersion;
        // TODO: URL ENCODING
        mDispatchURL = url;
    }
    
    std::string Hit::getDispatchURL() const{
        std::string full_dispatch_url = mDispatchURL;
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