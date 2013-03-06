#include <exception>
#include <string>
#include <map>


#include "TrackerImpl.h"
#include "Dispatcher.h"
#include "TrackerState.h"
#include "HitType.h"
#include "Transaction.h"
#include "Tracker.h"

namespace GAI { 

TrackerImpl::TrackerImpl(Dispatcher* aDispatcher, std::string& aTrackingID, std::string& aAppName, std::string& aAppVersion) {
}

bool TrackerImpl::sendView(std::string& aScreen) {
	throw "Not yet implemented";
}

bool TrackerImpl::sendEvent(std::string& aCategory, std::string& aAction, std::string& aLabel, std::string& aValue) {
	throw "Not yet implemented";
}

bool TrackerImpl::sendTransaction(Transaction* aTransaction) {
	throw "Not yet implemented";
}

bool TrackerImpl::sendException(bool aIsFatal, std::string& aDescription) {
	throw "Not yet implemented";
}

bool TrackerImpl::sendTimingWithCategory(std::string& aCategory, double aTime, std::string& aName, std::string& aLabel) {
	throw "Not yet implemented";
}

bool TrackerImpl::sendSocial(std::string& aNetwork, std::string& aAction, std::string& aTarget) {
	throw "Not yet implemented";
}

bool TrackerImpl::setParameter(std::string& aName, std::string& aValue) {
	throw "Not yet implemented";
}

std::string TrackerImpl::getParameter(std::string& aName) {
	throw "Not yet implemented";
}

bool TrackerImpl::sendParameters(std::string& aTrackType, std::map<std::string, std::string>& aParameters) {
	throw "Not yet implemented";
}

void TrackerImpl::close() {
	throw "Not yet implemented";
}

bool TrackerImpl::internalSend(HitType aType, std::map<std::string, std::string> aParameters) {
	throw "Not yet implemented";
}

}
