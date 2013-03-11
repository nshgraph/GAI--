#include <exception>
#include <string>
#include <map>


#include "RequestBuilder.h"
#include "Dispatcher.h"
#include "Hit.h"

namespace GAI
{

std::string RequestBuilder::requestForHit(Hit* aHit) {
	throw "Not yet implemented";
}

std::string RequestBuilder::encodeParameter(std::string& aName, std::string& aValue) {
	throw "Not yet implemented";
}

std::string RequestBuilder::encodeParameters(std::map<std::string, std::string>& aParameters) {
	throw "Not yet implemented";
}

}