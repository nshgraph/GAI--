#include <exception>
#include <string>


#include "ReachabilityChecker.h"
#include "ReachabilityDelegate.h"
#include "Dispatcher.h"

ReachabilityChecker::ReachabilityChecker(ReachabilityDelegate* aDelegate, std::string& aHost) {
}

void ReachabilityChecker::start() {
	throw "Not yet implemented";
}

void ReachabilityChecker::stop() {
	throw "Not yet implemented";
}

int32_t ReachabilityChecker::getStatus() {
	return this->_status;
}

