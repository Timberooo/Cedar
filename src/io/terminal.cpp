//
// This file makes heavy use of ANSI escape sequences, specifically the control sequence
// introducer (CSI) with the string "\033[".
// https://en.wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands
//

#include "terminal.h"

#include "../core.h"
#include "../math.h"

#include <cassert>
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
    constexpr DWORD cookedModeInputFlags = ENABLE_ECHO_INPUT |
                                           ENABLE_LINE_INPUT |
                                           ENABLE_PROCESSED_INPUT;

    // Necessary for controlling the terminal through ANSI escape sequences
    constexpr DWORD vtProcessingOutputModeFlags = ENABLE_PROCESSED_OUTPUT |
                                                  ENABLE_VIRTUAL_TERMINAL_PROCESSING;



    struct TerminalData
    {
        HANDLE inputHandle  = NULL;
        HANDLE outputHandle = NULL;

        bool altScreenBufferEnabled = false;


        inline TerminalData() {}

        inline ~TerminalData();
    };



    inline TerminalData::~TerminalData() {
        Cedar::Terminal::show(false);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// TODO: Implement Linux version.

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
namespace Cedar::Terminal
{
    void showCursor(bool show)
    {
        if (show)
            write("\033[?25h");
        else
            write("\033[?25l");
    }



    void moveCursor(MoveCursorDirection direction, int amount)
    {
        if (amount < 1)
            return;

        write("\033[" + std::to_string(amount) + (char)direction);
    }



    void moveCursor(Vector2D<int> amount)
    {
        Vector2D<MoveCursorDirection> direction;

        if (amount.x > 0)
            direction.x = MoveCursorDirection::right;
        else
        {
            direction.x = MoveCursorDirection::left;
            amount.x *= -1;
        }

        if (amount.y > 0)
            direction.y = MoveCursorDirection::down;
        else
        {
            direction.y = MoveCursorDirection::up;
            amount.y *= -1;
        }

        if (amount.x != 0)
            write("\033[" + std::to_string(amount.x) + (char)direction.x);
        if (amount.y != 0)
            write("\033[" + std::to_string(amount.y) + (char)direction.y);
    }



    void setCursorPosition(Point2D<int> position)
    {
        if (!visible())
            return;

        Size2D<int> terminalSize = size();

        position.x = clamp(position.x, 0, terminalSize.width - 1);
        position.y = clamp(position.y, 0, terminalSize.height - 1);

        write("\033[" + std::to_string(position.y + 1) + ';' + std::to_string(position.x + 1) + 'H');
    }



    void setForegroundColor(Color color)
    {
        write("\033[" + std::to_string((int)color) + 'm');
    }



    void setBackgroundColor(Color color)
    {
        write("\033[" + std::to_string(((int)color) + 10) + 'm');
    }



    void resetForegroundColor()
    {
        write("\033[39m");
    }



    void resetBackgroundColor()
    {
        write("\033[49m");
    }



    void resetColor()
    {
        write("\033[0m");
    }
}
// OS-agnostic implementation



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "../platform/windows.h"



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

            // Make sure VT procerssing is enabled
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

        g_terminalData.altScreenBufferEnabled = false;
    }



    bool visible()
    {
        assert((g_terminalData.outputHandle == NULL && g_terminalData.inputHandle == NULL) ||
               (g_terminalData.outputHandle != NULL && g_terminalData.inputHandle != NULL));

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



    Size2D<int> size()
    {
        if (!visible())
            return { 0, 0 };
        else
        {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            (void)GetConsoleScreenBufferInfo(g_terminalData.outputHandle, &csbi);

            return { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
        }
    }



    void enableAltScreenBuffer(bool enable)
    {
        if (!visible() || g_terminalData.altScreenBufferEnabled == enable)
            return;

        if (enable)
            write("\033[?1049h");
        else
            write("\033[?1094l");

        g_terminalData.altScreenBufferEnabled = enable;
    }



    void clear()
    {
        if (!visible())
            return;

        setCursorPosition(0, 0);
        resetColor();

        Size2D<int> terminalSize = size();

        COORD startCell = { 0, 0 };
        DWORD charsWritten;

        (void)FillConsoleOutputCharacterA(g_terminalData.outputHandle, ' ', terminalSize.width * terminalSize.height, startCell, &charsWritten);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// TODO: Implement Linux version.

#endif // ^^^ Linux ^^^
// OS-specific implementation