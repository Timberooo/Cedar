#include "terminal.h"

#include <cstdlib>
#include <exception>
#include <string>

// TODO: Remove once reading console input is implemented.
#include <unistd.h>



void logUnhandledException(const std::string& exceptionMsg);



int main(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::setMode(Cedar::Terminal::Mode::raw);
        Cedar::Terminal::enableAltScreenBuffer(true);
        Cedar::Terminal::showCursor(false);

        Cedar::Terminal::writeLine("Hello world! \u256B press 'Q' to quit");

        // TODO: Remove once reading console input is implemented.
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1 && !(c == 'q' || c == 'Q'));

        exitStatus = EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        logUnhandledException(e.what());
    }
    catch (...)
    {
        logUnhandledException("An unknown exception occurred");
    }

    return exitStatus;
}



void logUnhandledException(const std::string& exceptionMsg)
{
    Cedar::Terminal::enableAltScreenBuffer(false);
    Cedar::Terminal::writeLine("Unhandled exception: " + exceptionMsg);
}