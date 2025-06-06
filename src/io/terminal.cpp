#include "terminal.h"

#include "../core.h"
#include "../math.h"

#include <cstddef>
#include <new>
#include <string>
#include <string_view>
#include <type_traits>



namespace
{
    struct TerminalData;
}



// OS-specific definition of TerminalData
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "../platform/windows.h"



namespace
{
    struct TerminalData
    {
        HANDLE inputHandle  = NULL;
        HANDLE outputHandle = NULL;

        
        inline TerminalData() {}

        inline ~TerminalData();
    };



    inline TerminalData::~TerminalData() {
        Cedar::Terminal::show(false);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

namespace
{
    struct TerminalData {};
}

#endif // ^^^ Linux ^^^
// OS-specific definition of TerminalData



// Nifty counter internal details
namespace
{
    static typename std::aligned_storage<sizeof(TerminalData), alignof(TerminalData)>::type g_terminalDataBuffer;

    TerminalData& g_terminalData = reinterpret_cast<TerminalData&>(g_terminalDataBuffer);
}



namespace Cedar::Terminal
{
    std::size_t TerminalInitializer::s_counter = 0;



    TerminalInitializer::TerminalInitializer()
    {
        if (s_counter == 0)
            new (&g_terminalData)TerminalData();

        s_counter++;
    }



    TerminalInitializer::~TerminalInitializer()
    {
        s_counter--;

        if (s_counter == 0)
            g_terminalData.~TerminalData();
    }
}
// Nifty counter internal details



// OS-agnostic implementation
namespace
{
    enum class ColorType {
        foreground = 0,
        background = 10
    };



    void setColor(Cedar::Terminal::Color color, ColorType type);

    inline void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor);

    inline void resetColors();



    void setColor(Cedar::Terminal::Color color, ColorType type)
    {
        if (color != Cedar::Terminal::Color::use_default)
            Cedar::Terminal::write("\033[" + std::to_string(((int)color) + ((int)type)) + 'm');
    }



    inline void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor) {
        setColor(foregroundColor, ColorType::foreground);
        setColor(backgroundColor, ColorType::background);
    }



    inline void resetColors() {
        Cedar::Terminal::write("\033[0m");
    }
}



namespace Cedar::Terminal
{
    void write(std::string_view str, Color foregroundColor, Color backgroundColor)
    {
        setColors(foregroundColor, backgroundColor);
        write(str);
        resetColors();
    }

    void write(char character, Color foregroundColor, Color backgroundColor)
    {
        setColors(foregroundColor, backgroundColor);
        write(character);
        resetColors();
    }
}
// OS-agnostic implementation



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "../platform/windows.h"



namespace
{
    // Necessary for controlling the terminal through ANSI escape sequences
    constexpr DWORD vtProcessingOutputModeFlags = ENABLE_PROCESSED_OUTPUT |
                                                  ENABLE_VIRTUAL_TERMINAL_PROCESSING;
}



namespace Cedar::Terminal
{
    void show(bool showTerminal)
    {
        if (showTerminal == visible())
            return;

        if (showTerminal)
        {
            // TODO: Check result of AllocConsole.
            (void)AllocConsole();
            g_terminalData.inputHandle = GetStdHandle(STD_INPUT_HANDLE);
            g_terminalData.outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

            // Make sure VT processing is enabled
            DWORD outputMode;
            (void)GetConsoleMode(g_terminalData.outputHandle, &outputMode);
            outputMode |= vtProcessingOutputModeFlags;
            (void)SetConsoleMode(g_terminalData.outputHandle, outputMode);
        }
        else
        {
            // TODO: Free result of AllocConsole.
            (void)FreeConsole();
            g_terminalData.inputHandle = NULL;
            g_terminalData.outputHandle = NULL;
        }
    }



    bool visible()
    {
        return g_terminalData.outputHandle != NULL;
    }



    void write(std::string_view str)
    {
        if (visible())
            (void)WriteConsoleA(g_terminalData.outputHandle, str.data(), str.length(), NULL, NULL);
    }

    void write(char character)
    {
        if (visible())
            (void)WriteConsoleA(g_terminalData.outputHandle, &character, 1, NULL, NULL);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

#include <unistd.h>



namespace Cedar::Terminal
{
    void show(bool showTerminal)
    {
        // NOTE: This function and visible() only exist because of how Windows' console
        //       vs window subsystem works. These functions aren't necessary for Linux.
    }



    bool visible()
    {
        // NOTE: This function and show() only exist because of how Windows' console vs
        //       window subsystem works. These functions aren't necessary for Linux.
        return true;
    }



    void write(std::string_view str)
    {
        (void)::write(STDOUT_FILENO, str.data(), str.length());
    }

    void write(char character)
    {
        (void)::write(STDOUT_FILENO, &character, 1);
    }
}

#endif // ^^^ Linux ^^^
// OS-specific implementation