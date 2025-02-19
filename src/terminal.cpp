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

// TODO: Implement Windows version.

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

// TODO: Implement Windows version.

#endif // ^^^ Windows ^^^