#include "common_main.h"

#include "../terminal.h"
#include "../math.h"

#include "../gui/anchor.h"
#include "../gui/box.h"
#include "../gui/ielement.h"

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
        std::shared_ptr<Cedar::GUI::LayoutLayer> baseLayer = std::make_shared<Cedar::GUI::LayoutLayer>();

        std::shared_ptr<Cedar::GUI::Box> box = baseLayer->addChild<Cedar::GUI::Box>();

        box->setBackgroundColor(Cedar::Color::blue);
        box->setAbsoluteWidth(10);
        box->setAbsoluteHeight(3);
        box->setRelativeX(0.25f);
        box->setAnchor(Cedar::GUI::Anchor::left);

        Cedar::Rectangle<int> windowBounds;
        windowBounds.topLeft.x = 0;
        windowBounds.topLeft.y = 0;
        windowBounds.size = Cedar::Terminal::size();

        baseLayer->render(windowBounds.size, windowBounds);
    }
}