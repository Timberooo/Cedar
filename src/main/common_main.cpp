#include "common_main.h"

#include <cstdlib>
#include <exception>



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        // Put code here

        exitStatus = EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        // TODO: Log exception message.
    }
    catch (...) {
        // TODO: Log unknown exception message.
    }

    return exitStatus;
}