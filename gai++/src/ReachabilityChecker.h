#include <exception>
#include <string>


#ifndef __ReachabilityChecker_h__
#define __ReachabilityChecker_h__

// #include "ReachabilityDelegate.h"
// #include "Dispatcher.h"

namespace GAI
{
	class ReachabilityDelegate;
	class Dispatcher;
	class ReachabilityChecker;
}

namespace GAI
{
	class ReachabilityChecker
	{
		 ReachabilityDelegate* _reachabilityDelegate;
		 std::string _host;
		 int32_t _status;
		 bool _isActive;
		Dispatcher* _unnamed_Dispatcher_;
		ReachabilityDelegate* _unnamed_ReachabilityDelegate_;

		ReachabilityChecker(ReachabilityDelegate* aDelegate, std::string& aHost);

		void start();

		void stop();

		int32_t getStatus();
	};
}

#endif
