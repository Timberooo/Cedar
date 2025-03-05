#include "common_main.h"

#include "../terminal.h"
#include "../math.h"

#include "../gui/anchor.h"

#include <cstdlib>
#include <exception>
#include <string>



namespace
{
    void logUnhandledException(const std::string& exceptionMsg);

    void unicodeByteTest();

    void array2DTest();
}



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::setMode(Cedar::Terminal::Mode::raw);
        Cedar::Terminal::enableAltScreenBuffer(true);
        Cedar::Terminal::showCursor(false);

        array2DTest();
        unicodeByteTest();

        Cedar::Terminal::writeLine("Hello world! press 'Q' to quit");

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



    void array2DTest()
    {
        Cedar::Array2D<Cedar::Color> guiTest(4, 4);
        int color = 29;

        for (std::size_t y = 0; y < guiTest.size().height; y++)
        {
            for (std::size_t x = 0; x < guiTest.size().width; x++)
            {
                if (color == 37)
                    color = 90;
                else if (color == 97)
                    color = 30;
                else
                    color++;

                guiTest.at(x, y) = (Cedar::Color)color;
            }
        }

        guiTest.resize(16, 2, Cedar::Color::bright_blue);

        for (std::size_t y = 0; y < guiTest.size().height; y++)
        {
            for (std::size_t x = 0; x < guiTest.size().width; x++)
            {
                Cedar::Terminal::setCursorPosition(x, y);
                Cedar::Terminal::setBackgroundColor(guiTest.at(x, y));
                Cedar::Terminal::write(' ');
            }
        }

        Cedar::Terminal::resetBackgroundColor();
        Cedar::Terminal::write('\n');
    }
}