#include "common_main.h"

#include "../terminal.h"

#include "../gui/anchor.h"

#include <cstdlib>
#include <exception>
#include <string>



namespace
{
    void logUnhandledException(const std::string& exceptionMsg);

    void unicodeByteTest();
}



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::setMode(Cedar::Terminal::Mode::raw);
        Cedar::Terminal::enableAltScreenBuffer(true);
        Cedar::Terminal::showCursor(false);

        Cedar::Terminal::writeLine("Hello world! press 'Q' to quit");
        unicodeByteTest();

        char input;

        do {
            input = Cedar::Terminal::getRawInput();

            if (input != '\0')
                Cedar::Terminal::write(std::to_string(input) + " ");
        } while (input != 'q' && input != 'Q');

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



    void unicodeByteTest()
    {
        const char* unicodeTest = "\u256b";

        Cedar::Terminal::write("Unicode byte test for " + std::string(unicodeTest) + ": ");

        for (std::size_t i = 0; unicodeTest[i] != '\0'; i++)
            Cedar::Terminal::write(std::to_string(unicodeTest[i]) + " ");

        Cedar::Terminal::write('\n');
    }
}