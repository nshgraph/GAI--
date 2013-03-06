#include <string>
#include <map>


#ifndef __Hit_h__
#define __Hit_h__

// #include "RequestBuilder.h"
// #include "HitBuilder.h"

namespace GAI
{
	class RequestBuilder;
	class HitBuilder;
	class Hit;
}

namespace GAI
{
	class Hit
	{
		 std::map<std::string, std::string> _parameters;
		 std::string _dispatchURL;
		 std::string _gaiVersion;
		 double _timestamp;
		RequestBuilder* _unnamed_RequestBuilder_;
		HitBuilder* _unnamed_HitBuilder_;

		void setParameters(std::map<std::string, std::string> aParameters);

		std::map<std::string, std::string> getParameters();

		void setDispatchURL(std::string aDispatchURL);

		std::string getDispatchURL();

		void setGaiVersion(std::string aGaiVersion);

		std::string getGaiVersion();

		void setTimestamp(double aTimestamp);

		double getTimestamp();
	};
}

#endif
