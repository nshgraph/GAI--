//
//  Platform.cpp
//  gai
//
//  Created by Nathan Holmberg on 17/04/13.
//
//

#include "Platform.h"

#include <windows.h>
#include <winternl.h>
#include <string>
#include <sstream>

/* File-local */
namespace
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	typedef NTSTATUS (WINAPI* _NtQueryInformationProcess)
		(_In_ HANDLE ProcessHandle, _In_ PROCESSINFOCLASS ProcessInformationClass,
		_Out_ PVOID ProcessInformation, _In_ ULONG ProcessInformationLength, _Out_opt_ PULONG ReturnLength);

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

	DWORD GetProcessPEBAddress(HANDLE hProc)
	{
		PROCESS_BASIC_INFORMATION peb;
		DWORD tmp;
		_NtQueryInformationProcess NtQueryInformationProcess_ = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationProcess");
		NtQueryInformationProcess_(hProc, ProcessBasicInformation, &peb, sizeof(PROCESS_BASIC_INFORMATION), &tmp);
		return (DWORD)peb.PebBaseAddress;
	}

	void OSVersion( DWORD* major, DWORD* minor )
	{
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
		DWORD pebAddress = GetProcessPEBAddress(handle);
		DWORD OSMajorVersionAddress = pebAddress + 0x0a4;
		DWORD OSMinorVersionAddress = pebAddress + 0x0a8;
		ReadProcessMemory(handle, (void*)OSMajorVersionAddress, major, sizeof(*major), 0);
		ReadProcessMemory(handle, (void*)OSMinorVersionAddress, minor, sizeof(*minor), 0);
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