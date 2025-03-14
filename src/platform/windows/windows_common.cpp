#include "windows_common.h"

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

        int strLength  = str.length();
        int wstrLength = MultiByteToWideChar(CP_UTF8, 0, str.data(), strLength, NULL, 0);

        if (wstrLength == 0)
            return false;

        wstr.resize(wstrLength);

        return MultiByteToWideChar(CP_UTF8, 0, str.data(), strLength, wstr.data(), wstrLength) != 0;
    }



    bool convertWideStringToString(std::wstring_view wstr, std::string& str)
    {
        // NOTE: WideCharToMultiByte fails if the length of wstr is 0, but it should be
        //       considered a success instead.
        if (wstr.length() == 0)
        {
            str = "";
            return true;
        }

        int wstrLength = wstr.length();
        int strLength = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstrLength, NULL, 0, NULL, NULL);

        if (strLength == 0)
            return false;

        str.resize(strLength);

        return WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstrLength, str.data(), strLength, NULL, NULL) != 0;
    }
}



namespace Cedar::Platform::Windows
{
    bool tryStringToWideString(std::string_view str, std::wstring wstr)
    {
        // TODO: Log error before returning if conversion fails.
        return convertStringToWideString(str, wstr);
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
        // TODO: Log error before returning if conversion fails.
        return convertWideStringToString(wstr, str);
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