#include "common_main.h"

#include "../io/log.h"
#include "../io/terminal.h"
#include "../window.h"

#include <cstdlib>
#include <exception>

#include <string>



void windowClosedCallback()
{
    Cedar::Log::trace("Window closed");
}



bool windowClosingCallback()
{
    Cedar::Log::trace("Window closing");
    return true;
}



void windowResizedCallback()
{
    Cedar::Log::trace("Window resized");
}



void visibilityChangedCallback()
{
    Cedar::Log::trace("Visibility changed");
}



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Log::setMinLevel(Cedar::Log::Level::Trace);

        CEDAR_LOG_TRACE("Entered commonMain");

        CEDAR_LOG_TRACE("trace message");
        CEDAR_LOG_DEBUG("debug message");
        CEDAR_LOG_INFO("info message");
        CEDAR_LOG_WARNING("warning message");
        CEDAR_LOG_ERROR("error message");
        CEDAR_LOG_CRITICAL("critical message");
        CEDAR_LOG_FATAL("fatal message");

        exitStatus = EXIT_SUCCESS;

        Cedar::Window::setClosedCallback(windowClosedCallback);
        Cedar::Window::setClosingCallback(windowClosingCallback);
        Cedar::Window::setResizedCallback(windowResizedCallback);
        Cedar::Window::setVisibilityChangedCallback(visibilityChangedCallback);

        Cedar::Window::open(Cedar::Window::OpenArgs().sizeLimits(200, 200, -1, -1).title("Cedar Engine").visibility(Cedar::Window::Visibility::Maximize));

        while (Cedar::Window::isOpen())
        {
            //Cedar::Log::trace("Polling events");
            Cedar::Window::getVisibility();
            Cedar::Window::pollEvents();
        }

        Cedar::Log::trace("Program terminating");
    }
    catch (const std::exception& e) {
        Cedar::Log::fatal(e.what());
    }
    catch (...) {
        Cedar::Log::fatal("An unknown exception occurred");
    }

    return exitStatus;
}