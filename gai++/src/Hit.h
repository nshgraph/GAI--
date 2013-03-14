#include <string>
#include <map>


#ifndef __Hit_h__
#define __Hit_h__

namespace GAI
{
	class HitBuilder;
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
        Hit();
        std::string mDispatchURL;
        std::string mGaiVersion;
        double mTimestamp;
	};
}

#endif
