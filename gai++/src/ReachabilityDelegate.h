#include <exception>


#ifndef __ReachabilityDelegate_h__
#define __ReachabilityDelegate_h__

namespace GAI
{
	class ReachabilityChecker;
    
	class ReachabilityDelegate
	{
		public:
        virtual void reachabilityStatusChanged(ReachabilityChecker* aReachability, int aStatus) = 0;
	};
}

#endif
