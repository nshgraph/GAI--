//
//  Platform.cpp
//  gai
//
//  Created by Nathan Holmberg on 17/04/13.
//
//

#include "Platform.h"

#include <sstream>

#include <CoreServices/CoreServices.h>
#include <sys/types.h>



namespace GAI
{
    std::string Platform::GetPlatformVersionString( )
    {
        SInt32 major_version, minor_version, bugfix_version;
            Gestalt(gestaltSystemVersionMajor,
                    reinterpret_cast<SInt32*>(&major_version));
            Gestalt(gestaltSystemVersionMinor,
                    reinterpret_cast<SInt32*>(&minor_version));
            Gestalt(gestaltSystemVersionBugFix,
                    reinterpret_cast<SInt32*>(&bugfix_version));
        std::stringstream ss;
        ss << "Intel Mac OS X " << major_version << "_" << minor_version << "_" << bugfix_version;
        return ss.str();
    }
    
    std::string Platform::GetLocale( )
    {
        return "en";
    }
}