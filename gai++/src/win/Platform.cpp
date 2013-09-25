//
//  Platform.cpp
//  gai
//
//  Created by Nathan Holmberg on 17/04/13.
//
//

#include "Platform.h"

#include <windows.h>
#include <string>
#include <sstream>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

bool PlatformIs64Bit()
{
#if _WIN64
	return true;
#else
	BOOL isWow64 = FALSE;
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	
	if(fnIsWow64Process)
	{
		if(!fnIsWow64Process(GetCurrentProcess(), &isWow64))
		{
			return false;
		}
	}
	
	return isWow64;
#endif
}

namespace GAI
{
    std::string Platform::GetPlatformUserAgentString( )
    {
        std::stringstream ss;
        ss << " (U; "<< GetPlatformVersionString() <<")";
        return ss.str();
    }
	
	std::string Platform::GetPlatformVersionString()
	{
        OSVERSIONINFO osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        
        GetVersionEx(&osvi);
        
        
        std::stringstream ss;
        ss << "Windows NT " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
		if( PlatformIs64Bit() )
			ss << "; Win64; x64";
        return ss.str();
	}

    std::string Platform::GetUserLanguage()
	{
        char buf[19];
        int ccBuf = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, (LPWSTR)buf, 9);
        buf[ccBuf++] = '-';
        ccBuf += GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, (LPWSTR)buf+ccBuf, 9);
        
		return std::string(buf);
	}

    std::string Platform::GetScreenResolution()
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