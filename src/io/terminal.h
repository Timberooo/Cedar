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
        use_default    = 0,
        black          = 30,
        red            = 31,
        green          = 32,
        yellow         = 33,
        blue           = 34,
        magenta        = 35,
        cyan           = 36,
        white          = 37,
        bright_black   = 90,
        bright_red     = 91,
        bright_green   = 92,
        bright_yellow  = 93,
        bright_blue    = 94,
        bright_magenta = 95,
        bright_cyan    = 96,
        bright_white   = 97,
    };



    void show(bool showTerminal);

    bool visible();



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