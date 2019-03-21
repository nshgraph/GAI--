
#include "URLBuilder.h"

#include <algorithm>
#include <string>
#include <sstream>

#include "GAIDefines.h"
#include "Hit.h"
#include "Timestamp.h"

namespace
{
	const uint64_t MAXIMUM_QUEUE_TIME = 14400000; // Four hours in milliseconds
}

namespace GAI
{
   	std::string UrlBuilder::createPOSTURL(const Hit& hit)
    {
        return kGAIURLPage;
    }
    
    std::string UrlBuilder::createPOSTPayload(const Hit& hit, const uint64_t now_timestamp)
    {
		// Calculate the hits queue time (latent time). Measurement Protocol states
		// that values greater than four hours may lead to hits not being processed
		// so constraints are applied to favour a recorded hit over an accurate hit.
		uint64_t queue_time = std::min(now_timestamp - hit.getTimestamp(), MAXIMUM_QUEUE_TIME);

		std::stringstream ss;
        ss << hit.getDispatchURL();
		ss << "&" << kQueueTimeModelKey << "=" << queue_time;
		ss << "&" << kCacheBusterModelKey << "=" << rand();
		return ss.str();
    }
}
