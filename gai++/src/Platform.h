//
//  Platform.h
//  gai
//
//  Created by Nathan Holmberg on 17/04/13.
//
//

#ifndef __gai__Platform__
#define __gai__Platform__

#include <string>

namespace GAI
{
	class Platform
	{
    public:
        static std::string GetPlatformVersionString( );
        static std::string GetLocale( );
    };
};


#endif /* defined(__gai__Platform__) */
