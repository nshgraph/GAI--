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
#include <vector>

/* File-local */
namespace
{
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

	void OSVersion( DWORD* major, DWORD* minor )
	{
		LPCWSTR kernel32 = L"kernel32.dll";
		DWORD dummy;
		DWORD file_version_info_size = ::GetFileVersionInfoSizeExW( FILE_VER_GET_NEUTRAL, kernel32, &dummy );
		std::vector<char> buffer( file_version_info_size );
		::GetFileVersionInfoExW( FILE_VER_GET_NEUTRAL, kernel32, 0, file_version_info_size, &buffer[ 0 ] );
		void *fixed_file_info_ptr = nullptr;
		UINT size = 0;
		::VerQueryValueW( buffer.data(), L"\\", &fixed_file_info_ptr, &size );
		VS_FIXEDFILEINFO* fixed_file_info = static_cast<VS_FIXEDFILEINFO *>( fixed_file_info_ptr );
		*major = HIWORD( fixed_file_info->dwFileVersionMS );
		*minor = LOWORD( fixed_file_info->dwFileVersionMS );
	}
}

namespace GAI
{
    std::string Platform::GetPlatformUserAgentString()
    {
        std::stringstream ss;
        ss << " (U; "<< GetPlatformVersionString() <<")";
        return ss.str();
    }
    
    std::string Platform::GetPlatformVersionString()
    {
		DWORD major;
		DWORD minor;
		OSVersion(&major, &minor);

        std::stringstream ss;
        ss << "Windows NT " << major << "." << minor;
        if( PlatformIs64Bit() )
            ss << "; Win64; x64";
        return ss.str();
    }

    std::string Platform::GetUserLanguage()
    {
        char buf[19];
        int ccBuf = GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, (LPSTR)buf, 9);

        // ccBuf is the number of bytes used INCLUDING null terminator
        buf[ccBuf-1] = '-';
        // do not need to increment ccBuf here, but we should ensure that it is zero in case the next call returns nothing
        buf[ccBuf] = 0;
        ccBuf += GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, (LPSTR)buf+ccBuf, 9);
        
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