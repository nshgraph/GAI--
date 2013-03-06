#include <exception>
#include <string>
#include <map>


#include "Tracker.h"
#include "GAI.h"
#include "Transaction.h"

namespace GAI
{

std::string Tracker::getTrackingId() {
	return this->_trackingId;
}

void Tracker::setAppName(std::string aAppName) {
	this->_appName = aAppName;
}

std::string Tracker::getAppName() {
	return this->_appName;
}

void Tracker::setAppId(std::string aAppId) {
	this->_appId = aAppId;
}

std::string Tracker::getAppId() {
	return this->_appId;
}

void Tracker::setAppVersion(std::string aAppVersion) {
	this->_appVersion = aAppVersion;
}

std::string Tracker::getAppVersion() {
	return this->_appVersion;
}

void Tracker::setAnonymize(bool aAnonymize) {
	this->_anonymize = aAnonymize;
}

bool Tracker::isAnonymize() {
	return this->_anonymize;
}

void Tracker::setUseHttps(bool aUseHttps) {
	this->_useHttps = aUseHttps;
}

bool Tracker::isUseHttps() {
	return this->_useHttps;
}

void Tracker::setSampleRate(double aSampleRate) {
	this->_sampleRate = aSampleRate;
}

double Tracker::getSampleRate() {
	return this->_sampleRate;
}

std::string Tracker::getClientId() {
	return this->_clientId;
}

void Tracker::setReferrerUrl(std::string aReferrerUrl) {
	this->_referrerUrl = aReferrerUrl;
}

std::string Tracker::getReferrerUrl() {
	return this->_referrerUrl;
}

void Tracker::setCampaignUrl(std::string aCampaignUrl) {
	this->_campaignUrl = aCampaignUrl;
}

std::string Tracker::getCampaignUrl() {
	return this->_campaignUrl;
}

void Tracker::setSessionStart(bool aSessionStart) {
	this->_sessionStart = aSessionStart;
}

bool Tracker::isSessionStart() {
	return this->_sessionStart;
}

void Tracker::setSessionTimeout(double aSessionTimeout) {
	this->_sessionTimeout = aSessionTimeout;
}

double Tracker::getSessionTimeout() {
	return this->_sessionTimeout;
}

Tracker::Tracker() {
	_sessionTimeout = 30;
}
}

