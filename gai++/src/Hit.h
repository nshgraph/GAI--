
#ifndef __Hit_h__
#define __Hit_h__

#include <string>
#include <map>

#include "GAIDefines.h"

namespace GAI
{
	class HitBuilder;
    class DataStore;
	class Hit
	{
    public:
		void setParameters( const std::map<std::string, std::string> parameters );

		std::string getDispatchURL() const;

		std::string getGaiVersion() const;

		void setTimestamp( double timestamp );

		double getTimestamp() const;
		
    protected:
        friend HitBuilder;
        friend DataStore;
		
        Hit();
        Hit( const std::string& version, const std::string& url, const double timestamp );
		
        std::string mDispatchURL;
        std::string mGaiVersion;
        double		mTimestamp;
		
	};
}

#endif
