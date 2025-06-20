//
// OS-Agnostic support for outputting to the terminal.
//

#ifndef CEDAR_IO_TERMINAL_H
#define CEDAR_IO_TERMINAL_H

#include "../math.h"

#include <cstddef>
#include <string_view>



namespace Cedar::Terminal
{
    // Nifty counter. For internal use only
    class TerminalInitializer
    {
    public:

        TerminalInitializer();

        ~TerminalInitializer();

    private:

        static std::size_t s_counter;
    };



    // Nifty counter. For internal use only
    static TerminalInitializer terminalInitializer;



    enum class Color {
        Use_Default    = 0,
        Black          = 30,
        Red            = 31,
        Green          = 32,
        Yellow         = 33,
        Blue           = 34,
        Magenta        = 35,
        Cyan           = 36,
        White          = 37,
        Bright_Black   = 90,
        Bright_Red     = 91,
        Bright_Green   = 92,
        Bright_Yellow  = 93,
        Bright_Blue    = 94,
        Bright_Magenta = 95,
        Bright_Cyan    = 96,
        Bright_White   = 97,
    };



    void enable(bool enableTerminal);

    bool enabled();



    void write(std::string_view str);

    void write(char character);

    void write(std::string_view str, Color foregroundColor, Color backgroundColor);

    void write(char character, Color foregroundColor, Color backgroundColor);


    inline void writeLine(std::string_view str);

    inline void writeLine(char character);

    inline void writeLine(std::string_view str, Color foregroundColor, Color backgroundColor);

    inline void writeLine(char character, Color foregroundColor, Color backgroundColor);



    inline void writeLine(std::string_view str) {
        write(str);
        write('\n');
    }

    inline void writeLine(char character) {
        write(character);
        write('\n');
    }

    inline void writeLine(std::string_view str, Color foregroundColor, Color backgroundColor) {
        write(str, foregroundColor, backgroundColor);
        write('\n');
    }

    inline void writeLine(char character, Color foregroundColor, Color backgroundColor) {
        write(character, foregroundColor, backgroundColor);
        write('\n');
    }
}

#endif // CEDAR_IO_TERMINAL_H