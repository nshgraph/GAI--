#include <exception>
#include <string>
#include <map>


#ifndef __RequestBuilder_h__
#define __RequestBuilder_h__

// #include "Dispatcher.h"
// #include "Hit.h"

namespace GAI
{
	class Dispatcher;
	class Hit;
	class RequestBuilder;
}

namespace GAI
{
	class RequestBuilder
	{
		 std::string _userAgent;
		Dispatcher* _unnamed_Dispatcher_;
		Hit* _unnamed_Hit_;

		std::string requestForHit(Hit* aHit);

		std::string encodeParameter(std::string& aName, std::string& aValue);

		std::string encodeParameters(std::map<std::string, std::string>& aParameters);
	};
}

#endif
