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
        std::string str = Cedar::Platform::Windows::wideStringToString(argv[i]);

        std::size_t argLength = str.length() + 1;
        argvConverted[i] = new char[argLength];
        strncpy_s(argvConverted[i], argLength, str.c_str(), argLength);
    }

    return argvConverted;
}



void freeArgs(int argc, char* argvConverted[])
{
    for (int i = 0; i < argc; i++)
        delete argvConverted[i];

    delete[] argvConverted;
}