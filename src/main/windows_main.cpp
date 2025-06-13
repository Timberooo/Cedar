#include "common_main.h"

#include "../platform/windows.h"

#include <cstddef>
#include <string>
#include <string.h>



char** convertArgs(int argc, wchar_t* argv[]);

void freeArgs(int argc, char* argvConverted[]);



int wmain(int argc, wchar_t* argv[])
{
    char** argvConverted = convertArgs(argc, argv);
    int exitStatus = commonMain(argc, argvConverted);
    freeArgs(argc, argvConverted);

    return exitStatus;
}



char** convertArgs(int argc, wchar_t* argv[])
{
    char** argvConverted = new char*[argc];

    for (int i = 0; i < argc; i++)
    {
        // Using the try version of string conversion to minimize
        // possible exceptions before commonMain is entered
        std::string str;
        Cedar::Platform::Windows::tryWideStringToString(argv[i], str);

        std::size_t argLength = str.length() + 1;
        argvConverted[i] = new char[argLength];
        (void)strncpy_s(argvConverted[i], argLength, str.c_str(), argLength);
    }

    return argvConverted;
}



void freeArgs(int argc, char* argvConverted[])
{
    for (int i = 0; i < argc; i++)
        delete argvConverted[i];

    delete[] argvConverted;
}