
#ifndef __Hit_h__
#define __Hit_h__

#include <string>
#include <map>

namespace GAI
{
	class HitBuilder;
    class DataStore;
	class Hit
    ///
    /// This class represents a fully formed and ready for transmission 'Hit' on recipt by Google Analytic's Measurement
    /// Protocol the associated hit should be accepted and interpreted as an even on the user's account
    ///
	{
    public:
		void setParameters( const std::map<std::string, std::string> parameters );

		std::string getDispatchURL() const;

		std::string getGaiVersion() const;

		void setTimestamp( const uint64_t timestamp );

		uint64_t getTimestamp() const;
		
    protected:
        friend HitBuilder; // this is required as HitBuilder should be the only user-accessable method of creating a Hit
        friend DataStore; // this is required to allow DataStore's to deserialize Hits
		
        Hit();
		
        Hit( const std::string& version, const std::string& url, const uint64_t timestamp );
        std::string mDispatchURL; // stores the encoded url representing all the parameters associated with this hit
        std::string mGaiVersion; // the Google analytics version this hit's parameters adhere to
        uint64_t mTimestamp; // the timestamp for this hit, in milliseconds
	};
}

#endif
