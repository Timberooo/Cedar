#include "common_main.h"

#include "../gui.h"
#include "../math.h"
#include "../terminal.h"

#include <cstdlib>
#include <exception>
#include <string>



namespace
{
    void logUnhandledException(const std::string& exceptionMsg);

    void unicodeByteTest();

    void array2DTest();

    void guiTest();
}



int commonMain(int argc, char* argv[])
{
    int exitStatus = EXIT_FAILURE;

    try
    {
        Cedar::Terminal::setMode(Cedar::Terminal::Mode::raw);
        Cedar::Terminal::enableAltScreenBuffer(true);
        Cedar::Terminal::showCursor(false);

        //array2DTest();
        //guiTest();
        //unicodeByteTest();

        //Cedar::Terminal::writeLine("Hello world! press 'Q' to quit");



        auto window = std::make_shared<Cedar::GUI::Window>();

        auto background = window->addChild<Cedar::GUI::Box>();
        background->setBackgroundColor(Cedar::Color::black);

        auto menuLayer = window->addChild<Cedar::GUI::Layer>();
        menuLayer->setAbsoluteWidth(20);
        menuLayer->setAbsoluteHeight(11);

        auto menuBackground = menuLayer->addChild<Cedar::GUI::Box>();
        menuBackground->setBackgroundColor(Cedar::Color::white);



        char input;

        do {
            window->render();
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
        Cedar::Array2D<Cedar::Color> test;
        test.resize(5, 2, Cedar::Color::red);
        test.resize(1, 3);

        test.foreach([&](Cedar::Point2D<std::size_t> pos)
        {
            Cedar::Terminal::setBackgroundColor(test.at(pos.x, pos.y));
            Cedar::Terminal::write(' ');
        });
    }



    void guiTest()
    {
        auto window = std::make_shared<Cedar::GUI::Window>();

        auto background = window->addChild<Cedar::GUI::Box>();
        background->setBackgroundColor(Cedar::Color::black);

        auto menuLayer = window->addChild<Cedar::GUI::Layer>();
        menuLayer->setAbsoluteWidth(20);
        menuLayer->setAbsoluteHeight(11);

        auto menuBackground = menuLayer->addChild<Cedar::GUI::Box>();
        menuBackground->setBackgroundColor(Cedar::Color::white);

        window->render();



        //auto baseLayer = std::make_shared<Cedar::GUI::Layer>();

        //auto background = baseLayer->addChild<Cedar::GUI::Box>();
        //background->setBackgroundColor(Cedar::Color::black);

        //auto menuLayer = baseLayer->addChild<Cedar::GUI::Layer>();
        //menuLayer->setAbsoluteWidth(20);
        //menuLayer->setAbsoluteHeight(11);

        //auto menuBackground = menuLayer->addChild<Cedar::GUI::Box>();
        //menuBackground->setBackgroundColor(Cedar::Color::white);

        //auto menuOptionsTable = menuLayer->addChild<Cedar::GUI::Table>();
        //menuOptionsTable->resize(1, 4);
        //menuOptionsTable->setBackgroundColor(Cedar::Color::blue);
        //menuOptionsTable->setAbsoluteColumnSize(0, 10);
        //menuOptionsTable->setAbsoluteRowSize(0, 2);
        //menuOptionsTable->setAbsoluteRowSize(1, 2);
        //menuOptionsTable->setAbsoluteRowSize(2, 2);
        //menuOptionsTable->setAbsoluteRowSize(3, 2);

        //auto resumeButton = menuOptionsTable->setChild<Cedar::GUI::Box>(0, 0);
        //resumeButton->setAnchor(Cedar::GUI::Anchor::top);
        //resumeButton->setAbsoluteHeight(1);

        //auto newGameButton = menuOptionsTable->setChild<Cedar::GUI::Box>(0, 1);
        //newGameButton->setAnchor(Cedar::GUI::Anchor::top);
        //newGameButton->setAbsoluteHeight(1);

        //auto optionsButton = menuOptionsTable->setChild<Cedar::GUI::Box>(0, 2);
        //optionsButton->setAnchor(Cedar::GUI::Anchor::top);
        //optionsButton->setAbsoluteHeight(1);

        //auto quitButton = menuOptionsTable->setChild<Cedar::GUI::Box>(0, 3);
        //quitButton->setAnchor(Cedar::GUI::Anchor::top);
        //quitButton->setAbsoluteHeight(1);

        //baseLayer->startRender();
    }
}