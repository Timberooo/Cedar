#include "windows_common.h"

#include "../../io/log.h"

#include <string>
#include <string_view>
#include <system_error>



namespace
{
    bool convertStringToWideString(std::string_view str, std::wstring& wstr);

    bool convertWideStringToString(std::wstring_view wstr, std::string& str);



    bool convertStringToWideString(std::string_view str, std::wstring& wstr)
    {
        // NOTE: MultiByteToWideChar fails if the length of str is 0, but it should be
        //       considered a success instead.
        if (str.length() == 0)
        {
            wstr = L"";
            return true;
        }

        int wstrLength = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), NULL, 0);

        if (wstrLength == 0)
            return false;

        wstr.resize(wstrLength);

        return (MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), wstr.data(), wstrLength) != 0);
    }



    bool convertWideStringToString(std::wstring_view wstr, std::string& str)
    {
        // NOTE: WideCharToMultiByte fails of the length of wstr is 0, but it should be
        //       considered a success instead.
        if (wstr.length() == 0)
        {
            str = "";
            return true;
        }

        int strLength = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.length(), NULL, 0, NULL, NULL);

        if (strLength == 0)
            return false;

        str.resize(strLength);

        return (WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.length(), str.data(), strLength, NULL, NULL) != 0);
    }
}



namespace Cedar::Platform::Windows
{
    bool tryStringToWideString(std::string_view str, std::wstring& wstr)
    {
        bool result = convertStringToWideString(str, wstr);

        if (!result)
            Cedar::Log::error("Failed to convert string to wide string");

        return result;
    }



    std::wstring stringToWideString(std::string_view str)
    {
        std::wstring wstr;

        if (convertStringToWideString(str, wstr))
            return wstr;
        else
            throw std::system_error(GetLastError(), std::system_category(),
                                    "Failed to convert string to wide string");
    }



    bool tryWideStringToString(std::wstring_view wstr, std::string& str)
    {
        bool result = convertWideStringToString(wstr, str);

        if (!result)
            Cedar::Log::error("Failed to convert wide string to string");

        return result;
    }



    std::string wideStringToString(std::wstring_view wstr)
    {
        std::string str;

        if (convertWideStringToString(wstr, str))
            return str;
        else
            throw std::system_error(GetLastError(), std::system_category(),
                                    "Failed to convert wide string to string");
    }
}