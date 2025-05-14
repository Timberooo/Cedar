#include "common_main.h"

#include "../math.h"
#include "../io/terminal.h"

#include <cstdlib>
#include <exception>
#include <string>



namespace
{
    void logUnhandledException(const std::string& exceptionMsg);
}



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::setForegroundColor(Cedar::Terminal::Color::bright_green);
        Cedar::Terminal::writeLine("This is a test");

        exitStatus = EXIT_SUCCESS;
    }
    catch(const std::exception& e) {
        logUnhandledException(e.what());
    }
    catch (...) {
        logUnhandledException("An unknown exception occurred");
    }

    return exitStatus;
}



namespace
{
    void logUnhandledException(const std::string& exceptionMsg)
    {
        Cedar::Terminal::enableAltScreenBuffer(false);
        Cedar::Terminal::writeLine("Unhandled exception: " + exceptionMsg);
    }
}