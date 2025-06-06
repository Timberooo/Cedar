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
        Cedar::Terminal::enable(true);
        Cedar::Log::setMinLevel(Cedar::Log::Level::trace);

        CEDAR_LOG_TRACE("Entered commonMain");

        CEDAR_LOG_TRACE("trace message");
        CEDAR_LOG_DEBUG("debug message");
        CEDAR_LOG_INFO("info message");
        CEDAR_LOG_WARNING("warning message");
        CEDAR_LOG_ERROR("error message");
        CEDAR_LOG_CRITICAL("critical message");
        CEDAR_LOG_FATAL("fatal message");

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