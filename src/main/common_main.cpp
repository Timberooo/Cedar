#include "common_main.h"

#include "../io/log.h"
#include "../io/terminal.h"

#include <cstdlib>
#include <exception>

#include <string>



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::show(true);
        Cedar::Log::setMinLevel(Cedar::Log::Level::trace);

        CEDAR_LOG_TRACE("Entered commonMain");

        for (int i = 1; i < argc; i++)
        {
            Cedar::Log::debug("Command: " + std::string(argv[i]));

            if (argv[i] == "showconsole")
                Cedar::Log::info("Started with command \"showconsole\"");
        }

        exitStatus = EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        Cedar::Log::fatal(e.what());
    }
    catch (...) {
        Cedar::Log::fatal("An unknown exception occurred");
    }

    return exitStatus;
}