#include "terminal.h"

#include "core.h"
#include "math.h"

#include <cstddef>
#include <new>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>



namespace
{
    struct InternalData;
}



// OS-specific implementation of InternalData
#if defined(CEDAR_OS_LINUX) // vvv Linux vvv

#include <termios.h>
#include <unistd.h>



namespace
{
    // TODO: Check if the Windows console has these control sequences, and if it does
    //       find a way to disable them. If that's not possible these may have to be
    //       removed for parity.

    constexpr tcflag_t cookedModeInputFlags  = IXON   | // Ctrl-S and Ctrl-Q
                                               ICRNL;   // Ctrl-M

    constexpr tcflag_t cookedModeOutputFlags = ECHO   | // Prints typed keys to the terminal
                                               ICANON | // Read input line-by-line
                                               ISIG   | // Ctrl-C and Ctrl-Z
                                               IEXTEN;  // Ctrl-V

    // NOTE: Currently unused. Not sure Windows has an equivalent.
    constexpr tcflag_t newlineProcessingFlag = OPOST;   // Process "\n" as "\r\n"



    struct InternalData
    {
    public:

        Cedar::Terminal::Mode mode;
        bool                  altScreenBufferEnabled;

        const termios originalAttributes;


        InternalData();

        ~InternalData();

    private:

        inline static termios initOriginalAttributes();
    };



    InternalData::InternalData() : originalAttributes(initOriginalAttributes())
    {
        // NOTE: Can't be sure if the alt screen buffer is already enabled or not, but
        //       can reasonably assume its not.
        altScreenBufferEnabled = false;
        
        // Initializing the mode to raw so setMode will apply the cooked mode flags.
        mode = Cedar::Terminal::Mode::raw;
        Cedar::Terminal::setMode(Cedar::Terminal::Mode::cooked);
    }



    InternalData::~InternalData()
    {
        Cedar::Terminal::resetColor();
        
        // NOTE: Can't be sure what these were set to before the program started, but can
        //       reasonably assume they were set to these values.
        Cedar::Terminal::showCursor(true);
        Cedar::Terminal::enableAltScreenBuffer(false);

        (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalAttributes);
    }



    inline termios InternalData::initOriginalAttributes() {
        termios attributes;
        (void)tcgetattr(STDIN_FILENO, &attributes);
        return attributes;
    }
}

#elif defined(CEDAR_OS_WINDOWS) // vvv Windows vvv // ^^^ Linux ^^^

#include "platform/windows.h"



namespace
{
    constexpr DWORD cookedModeInputFlags = ENABLE_ECHO_INPUT |
                                           ENABLE_LINE_INPUT |
                                           ENABLE_PROCESSED_INPUT;

    constexpr DWORD vtProcessingOutputModeFlags = ENABLE_PROCESSED_OUTPUT |
                                                  ENABLE_VIRTUAL_TERMINAL_PROCESSING;



    struct InternalData
    {
    public:

        struct OriginalConsoleInfo
        {
            DWORD mode;
            UINT  codePage;
        };


        Cedar::Terminal::Mode mode;
        bool                  altScreenBufferEnabled;

        const HANDLE stdOutputHandle;
        const HANDLE stdInputHandle;

        const OriginalConsoleInfo originalOutputInfo;
        const OriginalConsoleInfo originalInputInfo;


        InternalData();

        ~InternalData();

    private:

        static OriginalConsoleInfo initOriginalConsoleInfo(HANDLE handle);
    };



    InternalData::InternalData() :
        stdOutputHandle(GetStdHandle(STD_OUTPUT_HANDLE)),
        stdInputHandle(GetStdHandle(STD_INPUT_HANDLE)),
        originalOutputInfo(initOriginalConsoleInfo(GetStdHandle(STD_OUTPUT_HANDLE))),
        originalInputInfo(initOriginalConsoleInfo(GetStdHandle(STD_INPUT_HANDLE)))
    {
        // NOTE: Can't be sure if the alt screen buffer is already enabled or not, but
        //       can reasonably assume its not.
        altScreenBufferEnabled = false;

        // Make sure VT processing is enabled
        DWORD outputMode;
        (void)GetConsoleMode(stdOutputHandle, &outputMode);
        outputMode |= vtProcessingOutputModeFlags;
        (void)SetConsoleMode(stdOutputHandle, outputMode);

        // Initializing the mode to raw so setMode will apply the cooked mode flags.
        mode = Cedar::Terminal::Mode::raw;
        Cedar::Terminal::setMode(Cedar::Terminal::Mode::cooked);

        (void)SetConsoleOutputCP(CP_UTF8);
        (void)SetConsoleCP(CP_UTF8);
    }



    InternalData::~InternalData()
    {
        Cedar::Terminal::resetColor();

        // NOTE: Can't be sure what these were set to before the program started, but can
        //       reasonably assume they were set to these values.
        Cedar::Terminal::showCursor(true);
        Cedar::Terminal::enableAltScreenBuffer(false);

        (void)SetConsoleMode(stdOutputHandle, originalOutputInfo.mode);
        (void)SetConsoleMode(stdInputHandle, originalInputInfo.mode);

        (void)SetConsoleOutputCP(originalOutputInfo.codePage);
        (void)SetConsoleCP(originalInputInfo.codePage);
    }



    InternalData::OriginalConsoleInfo InternalData::initOriginalConsoleInfo(HANDLE handle)
    {
        OriginalConsoleInfo originalConsoleInfo;

        (void)GetConsoleMode(handle, &originalConsoleInfo.mode);
        
        if (handle == GetStdHandle(STD_OUTPUT_HANDLE))
            originalConsoleInfo.codePage = GetConsoleOutputCP();
        else // Assumed to be STD_INPUT_HANDLE
            originalConsoleInfo.codePage = GetConsoleCP();

        return originalConsoleInfo;
    }
}

#endif // ^^^ Windows ^^^



namespace
{
    // TODO: Change this to std::byte[] instead of std::aligned_storage since
    //       std::aligned_storage is depreciated in C++23.

    static typename std::aligned_storage<sizeof(InternalData), alignof(InternalData)>::type g_internalDataBuff;

    InternalData& g_internalData = reinterpret_cast<InternalData&>(g_internalDataBuff);
}



namespace Cedar::Terminal
{
    std::size_t Initializer::s_counter = 0;



    Initializer::Initializer()
    {
        if (s_counter == 0)
            new (&g_internalData)InternalData();
        
        s_counter++;
    }



    Initializer::~Initializer()
    {
        s_counter--;

        if (s_counter == 0)
            g_internalData.~InternalData();
    }



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
            direction.y = MoveCursorDirection::up;
        else
        {
            direction.y = MoveCursorDirection::down;
            amount.y *= -1;
        }

        if (amount.x != 0)
            write("\033[" + std::to_string(amount.x) + (char)direction.x);
        if (amount.y != 0)
            write("\033[" + std::to_string(amount.y) + (char)direction.y);
    }



    void setCursorPosition(Point2D<int> position)
    {
        Size2D<int> terminalSize = size();

        position.x = clamp(position.x, 0, terminalSize.width - 1);
        position.y = clamp(position.y, 0, terminalSize.height - 1);

        // TODO: Figure out why the +1 is necessary.
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



// OS-specific implementation of terminal functions
#if defined(CEDAR_OS_LINUX) // vvv Linux vvv

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>



namespace Cedar::Terminal
{
    void write(std::string_view str)
    {
        (void)::write(STDOUT_FILENO, str.data(), str.size());
    }



    void write(char character)
    {
        (void)::write(STDOUT_FILENO, &character, 1);
    }



    char getRawInput()
    {
        if (getMode() != Mode::raw)
            throw std::runtime_error("Cannot get raw input while in cooked mode");

        char c = '\0';
        (void)read(STDIN_FILENO, &c, 1);

        return c;
    }



    Size2D<int> size()
    {
        winsize terminalSize;
        (void)ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);

        return { terminalSize.ws_col, terminalSize.ws_row };
    }



    Mode getMode()
    {
        // TODO: See if this can be a non-OS-specific function when the Windows version
        //       is implemented.
        return g_internalData.mode;
    }



    void setMode(Mode mode)
    {
        if (g_internalData.mode == mode)
            return;
        
        termios attributes;
        tcgetattr(STDIN_FILENO, &attributes);

        if (mode == Mode::cooked)
        {
            attributes.c_iflag |= cookedModeInputFlags;
            attributes.c_lflag |= cookedModeOutputFlags;
            attributes.c_cc[VMIN]  = g_internalData.originalAttributes.c_cc[VMIN];
            attributes.c_cc[VTIME] = g_internalData.originalAttributes.c_cc[VTIME];
        }
        else // Mode::raw
        {
            attributes.c_iflag &= ~cookedModeInputFlags;
            attributes.c_lflag &= ~cookedModeOutputFlags;
            attributes.c_cc[VMIN]  = 0; // read returns when any input is present
            attributes.c_cc[VTIME] = 1; // read waits for 100 milliseconds
        }
        
        (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);
        g_internalData.mode = mode;
    }



    void enableAltScreenBuffer(bool enable)
    {
        if (g_internalData.altScreenBufferEnabled == enable)
            return;
        
        if (enable)
            write("\033[?1049h");
        else
            write("\033[?1049l");

        g_internalData.altScreenBufferEnabled = enable;
    }



    void clear()
    {
        write("\033[2J");
    }
}

#elif defined(CEDAR_OS_WINDOWS) // vvv Windows vvv // ^^^ Linux ^^^

#include "platform/windows.h"



namespace Cedar::Terminal
{
    void write(std::string_view str)
    {
        (void)WriteConsoleA(g_internalData.stdOutputHandle, str.data(), str.length(), NULL, NULL);
    }

    void write(char character)
    {
        (void)WriteConsoleA(g_internalData.stdOutputHandle, &character, 1, NULL, NULL);
    }



    char getRawInput()
    {
        char input = '\0';

        DWORD eventCount;
        (void)GetNumberOfConsoleInputEvents(g_internalData.stdInputHandle, &eventCount);

        for (std::size_t i = 0; i < eventCount; i++)
        {
            DWORD        eventsRead;
            INPUT_RECORD inputRecord;
            (void)ReadConsoleInputA(g_internalData.stdInputHandle, &inputRecord, 1, &eventsRead);

            if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
                input = inputRecord.Event.KeyEvent.uChar.AsciiChar;
        }

        return input;
    }



    Size2D<int> size()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        (void)GetConsoleScreenBufferInfo(g_internalData.stdOutputHandle, &csbi);

        return { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
    }



    Mode getMode()
    {
        // TODO: See if this can be a non-OS-specific function.
        return g_internalData.mode;
    }



    void setMode(Mode mode)
    {
        if (g_internalData.mode == mode)
            return;

        DWORD inputMode;
        (void)GetConsoleMode(g_internalData.stdInputHandle, &inputMode);

        if (mode == Mode::cooked)
            inputMode |= cookedModeInputFlags;
        else // Mode::raw
            inputMode &= ~cookedModeInputFlags;

        (void)SetConsoleMode(g_internalData.stdInputHandle, inputMode);
        g_internalData.mode = mode;
    }



    void enableAltScreenBuffer(bool enable)
    {
        // TODO: See if this can be a non-OS-specific function.
        if (g_internalData.altScreenBufferEnabled == enable)
            return;

        if (enable)
            write("\033[?1049h");
        else
            write("\033[?1049l");

        g_internalData.altScreenBufferEnabled = enable;
    }



    void clear()
    {
        // NOTE: The ANSI escape sequence \033[2J doesn't seem to work on Windows so all
        //       of this is needed instead.

        setCursorPosition(0, 0);
        resetColor();

        Size2D<int> terminalSize = size();

        COORD startCell = { 0, 0 };
        DWORD charsWritten;

        (void)FillConsoleOutputCharacterA(g_internalData.stdOutputHandle, ' ', terminalSize.width * terminalSize.height, startCell, &charsWritten);
    }
}

#endif // ^^^ Windows ^^^