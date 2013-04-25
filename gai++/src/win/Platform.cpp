//
//  Platform.cpp
//  gai
//
//  Created by Nathan Holmberg on 17/04/13.
//
//

#include "Platform.h"
namespace GAI
{
	
	std::string Platform::GetPlatformVersionString( )
	{
		return "Windows";
	}
    std::string Platform::GetUserLanguage( )
	{
		return "en";
	}
    std::string Platform::GetScreenResolution( )
	{
		return "800x600";
	}
}