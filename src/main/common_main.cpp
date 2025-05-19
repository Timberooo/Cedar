#include "common_main.h"

#include "../io/log.h"
#include "../window.h"

#include <cstdlib>
#include <exception>



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Log::debug("debug message");
        Cedar::Log::info("info message");
        Cedar::Log::warning("warning message");
        Cedar::Log::error("error message");
        Cedar::Log::critical("critical message");
        Cedar::Log::fatal("fatal message");

        //Cedar::Window::Builder windowBuilder;

        //Cedar::Window window = windowBuilder.build();

        //while (Cedar::windowsOpen())
        //{
        //    Cedar::pollEvents();
        //}

        exitStatus = EXIT_SUCCESS;
    }
    catch(const std::exception& e) {
        Cedar::Log::fatal(e.what());
    }
    catch (...) {
        Cedar::Log::fatal("An unknown exception occurred");
    }

    return exitStatus;
}