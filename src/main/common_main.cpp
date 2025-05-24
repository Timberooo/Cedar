#include "common_main.h"

#include "../io/terminal.h"

#include <cstdlib>
#include <exception>



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::setForegroundColor(Cedar::Terminal::Color::bright_green);
        Cedar::Terminal::writeLine("This shouldn't be written since there's no terminal yet");

        Cedar::Terminal::show(true);
        Cedar::Terminal::setForegroundColor(Cedar::Terminal::Color::bright_magenta);
        Cedar::Terminal::writeLine("This should be visible. Hello!");

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