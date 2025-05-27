#include "common_main.h"

#include "../io/log.h"
#include "../io/terminal.h"
#include "../window.h"

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

        Cedar::Window::Builder winBuilder;
        winBuilder.title("Cedar Engine");

        Cedar::Window window = winBuilder.build();

        while (Cedar::windowsOpen())
        {
            Cedar::pollEvents();
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