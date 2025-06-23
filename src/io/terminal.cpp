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
        Cedar::Terminal::enable(false);
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

// OS-agnostic implementation



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "../platform/windows.h"



namespace
{
    // Necessary for controlling the terminal through ANSI escape sequences
    constexpr DWORD vtProcessingOutputModeFlags = ENABLE_PROCESSED_OUTPUT |
                                                  ENABLE_VIRTUAL_TERMINAL_PROCESSING;



    enum class ColorType : int {
        Foreground = 0,
        Background = 10
    };



    void writeInternal(std::string_view str);

    void writeInternal(char character);



    void setColor(Cedar::Terminal::Color color, ColorType type);

    void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor);

    void resetColors();



    void writeInternal(std::string_view str)
    {
        (void)WriteConsoleA(g_terminalData.outputHandle, str.data(), str.length(), NULL, NULL);
    }

    void writeInternal(char character)
    {
        (void)WriteConsoleA(g_terminalData.outputHandle, &character, 1, NULL, NULL);
    }



    void setColor(Cedar::Terminal::Color color, ColorType type)
    {
        if (color != Cedar::Terminal::Color::Use_Default)
            writeInternal("\033[" + std::to_string(((int)color) + ((int)type)) + 'm');
    }



    void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor)
    {
        setColor(foregroundColor, ColorType::Foreground);
        setColor(backgroundColor, ColorType::Background);
    }



    void resetColors()
    {
        writeInternal("\033[0m");
    }
}



namespace Cedar::Terminal
{
    void enable(bool enableTerminal)
    {
        if (enableTerminal == enabled())
            return;

        if (enableTerminal)
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



    bool enabled()
    {
        return g_terminalData.outputHandle != NULL;
    }



    void write(std::string_view str, Color foregroundColor, Color backgroundColor)
    {
        if (!enabled())
            return;

        setColors(foregroundColor, backgroundColor);
        writeInternal(str);
        resetColors();
    }



    void write(char character, Color foregroundColor, Color backgroundColor)
    {
        if (!enabled())
            return;

        setColors(foregroundColor, backgroundColor);
        writeInternal(character);
        resetColors();
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

#include <unistd.h>



namespace
{
    enum class ColorType : int {
        Foreground = 0,
        Background = 10
    };



    void writeInternal(std::string_view str);

    void writeInternal(char character);


    void setColor(Cedar::Terminal::Color color, ColorType type);

    void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor);

    void resetColors();



    void writeInternal(std::string_view str)
    {
        (void)::write(STDOUT_FILENO, str.data(), str.length());
    }



    void writeInternal(char character)
    {
        (void)::write(STDOUT_FILENO, &character, 1);
    }



    void setColor(Cedar::Terminal::Color color, ColorType type)
    {
        if (color != Cedar::Terminal::Color::Use_Default)
            writeInternal("\033[" + std::to_string(((int)color) + ((int)type)) + 'm');
    }



    void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor)
    {
        setColor(foregroundColor, ColorType::Foreground);
        setColor(backgroundColor, ColorType::Background);
    }



    void resetColors()
    {
        writeInternal("\033[0m");
    }
}



namespace Cedar::Terminal
{
    void enable(bool enableTerminal)
    {
        // NOTE: This function and enabled() only exist because of how Windows' console
        //       vs window subsystem works. These functions aren't necessary for Linux.
    }



    bool enabled()
    {
        // NOTE: This function and enable() only exist because of how Windows' console vs
        //       window subsystem works. These functions aren't necessary for Linux.
        return true;
    }



    void write(std::string_view str, Color foregroundColor, Color backgroundColor)
    {
        setColors(foregroundColor, backgroundColor);
        writeInternal(str);
        resetColors();
    }



    void write(char character, Color foregroundColor, Color backgroundColor)
    {
        setColors(foregroundColor, backgroundColor);
        writeInternal(character);
        resetColors();
    }
}

#endif // ^^^ Linux ^^^
// OS-specific implementation