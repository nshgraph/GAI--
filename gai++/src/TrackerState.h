#include <exception>
#include <string>


#ifndef __TrackerState_h__
#define __TrackerState_h__

// #include "Model.h"
// #include "TrackerImpl.h"

namespace GAI
{
	class Model;
	class TrackerImpl;
	class TrackerState;
}

namespace GAI
{
	struct TrackerState
	{
        Model* model;
        double sampleRate;
        bool sampled;
        std::string referrerUrl;
        std::string campaignUrl;
        std::string httpDispatchUrl;
        std::string httpsDispatchUrl;
        bool useHttps;
	};
}

#endif
