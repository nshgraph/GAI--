//
//  Platform.cpp
//  gai
//
//  Created by Nathan Holmberg on 17/04/13.
//
//

#include "Platform.h"
#include <windows.h>

namespace GAI
{
    std::string Platform::GetPlatformUserAgentString( )
    {
        std::stringstream ss;
        ss << " (U; "<< GetPlatformVersionString() <<")";
        return ss.str();
    }
	
	std::string Platform::GetPlatformVersionString( )
	{
        OSVERSIONINFO osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        
        GetVersionEx(&osvi);
        
        
        std::stringstream ss;
        ss << "Windows NT " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
        return ss.str();
	}
    std::string Platform::GetUserLanguage( )
	{
        char buf[19];
        int ccBuf = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, buf, 9);
        buf[ccBuf++] = '-';
        ccBuf += GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, buf+ccBuf, 9);
        
		return std::string(ccBuf);
	}
    std::string Platform::GetScreenResolution( )
	{
        RECT desktop;
        // Get a handle to the desktop window
        const HWND hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &desktop);
        
        std::stringstream ss;
        ss << desktop.right << "x" << desktop.bottom;
        return ss.str();
	}
}