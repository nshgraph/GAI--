#include <string>
#include <map>


#ifndef __Hit_h__
#define __Hit_h__

namespace GAI
{
	class HitBuilder;
    class DataStore;
	class Hit
	{
    public:
		void setParameters(std::map<std::string, std::string> aParameters);

		std::string getDispatchURL() const;

		std::string getGaiVersion() const;

		void setTimestamp(double aTimestamp);

		double getTimestamp() const;
    private:
        friend HitBuilder;
        friend DataStore;
        Hit();
        Hit(const std::string& version, const std::string& url, const double timestamp);
        std::string mDispatchURL;
        std::string mGaiVersion;
        double mTimestamp;
	};
}

#endif
