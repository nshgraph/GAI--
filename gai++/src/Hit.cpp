#include <string>
#include <map>


#include "Hit.h"
#include "RequestBuilder.h"
#include "HitBuilder.h"

namespace GAI
{

void Hit::setParameters(std::map<std::string, std::string> aParameters) {
	this->_parameters = aParameters;
}

std::map<std::string, std::string> Hit::getParameters() const{
	return this->_parameters;
}

std::string Hit::getDispatchURL() const{
	return this->_dispatchURL;
}

void Hit::setGaiVersion(std::string aGaiVersion) {
	this->_gaiVersion = aGaiVersion;
}

std::string Hit::getGaiVersion() const{
	return this->_gaiVersion;
}

void Hit::setTimestamp(double aTimestamp) {
	this->_timestamp = aTimestamp;
}

double Hit::getTimestamp() const{
	return this->_timestamp;
}

}