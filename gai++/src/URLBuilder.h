
#ifndef __UrlBuilder_h__
#define __UrlBuilder_h__

#include <string>

namespace GAI
{
	class Hit;

	class UrlBuilder
	{
    public:
		static std::string createPOSTURL(const Hit& hit);
		static std::string createPOSTPayload(const Hit& hit, const uint64_t now_timestamp);
    };
}

#endif
