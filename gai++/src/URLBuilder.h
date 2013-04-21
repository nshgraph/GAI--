

#ifndef __UrlBuilder_h__
#define __UrlBuilder_h__

#include <string>

#include "Hit.h"

namespace GAI
{
    
	class UrlBuilder
	{
    public:
		static std::string createURL(const Hit& hit);
        
		static std::string createPOSTURL(const Hit& hit);
		static std::string createPOSTPayload(const Hit& hit);
    protected:
        
	};
}

#endif
