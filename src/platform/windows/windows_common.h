//
// A collection of functions, data structures, includes, and preprocessor definitions
// that are commonly used throughout the Cedar engine when interfacing with the Windows
// API.
//
// Specific abstractions and supporting features that have no use outside of their
// intended files should be contained within those files in unnamed namespaces, but may
// also have their own files within the "platform/windows" directory.
//
// Including other headers within this directory in this file is forbidden, as that poses
// a risk for creating circular dependencies.
//

#ifndef CEDAR_PLATFORM_WINDOWS_WINDOWS_COMMON_H
#define CEDAR_PLATFORM_WINDOWS_WINDOWS_COMMON_H

#include <string>
#include <string_view>

// Excluding unnecessary Windows includes (also prevents conflicts between winsock.h and
// winsock2.h) and the windows min and max functions that conflict with min and max
// functions in the C++ standard library
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>



namespace Cedar::Platform::Windows
{
    bool tryStringToWideString(std::string_view str, std::wstring& wstr);

    std::wstring stringToWideString(std::string_view str);

    bool tryWideStringToString(std::wstring_view wstr, std::string& str);

    std::string wideStringToString(std::wstring_view wstr);
}

#endif // CEDAR_PLATFORM_WINDOWS_WINDOWS_COMMON_H