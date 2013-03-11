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

std::map<std::string, std::string> Hit::getParameters() {
	return this->_parameters;
}

void Hit::setDispatchURL(std::string aDispatchURL) {
	this->_dispatchURL = aDispatchURL;
}

std::string Hit::getDispatchURL() {
	return this->_dispatchURL;
}

void Hit::setGaiVersion(std::string aGaiVersion) {
	this->_gaiVersion = aGaiVersion;
}

std::string Hit::getGaiVersion() {
	return this->_gaiVersion;
}

void Hit::setTimestamp(double aTimestamp) {
	this->_timestamp = aTimestamp;
}

double Hit::getTimestamp() {
	return this->_timestamp;
}

}