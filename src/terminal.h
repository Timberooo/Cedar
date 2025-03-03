//
// Terminal.
//

#ifndef CEDAR_TERMINAL_H
#define CEDAR_TERMINAL_H

#include "color.h"
#include "math.h"

#include <cstddef>
#include <string_view>



namespace Cedar::Terminal
{
    enum class Mode {
        cooked,
        raw
    };

    enum class MoveCursorDirection {
        up    = 65,
        down  = 66,
        right = 67,
        left  = 68
    };


    class Initializer;


    void write(std::string_view str);

    void write(char character);

    inline void writeLine(std::string_view str);


    char getRawInput();


    Size2D<int> size();


    Mode getMode();

    void setMode(Mode mode);


    void enableAltScreenBuffer(bool enable);


    void showCursor(bool show);

    void moveCursor(MoveCursorDirection direction, int amount = 1);

    void moveCursor(Vector2D<int> amount);

    inline void moveCursor(int xAmount, int yAmount);

    void setCursorPosition(Point2D<int> position);

    inline void setCursorPosition(int x, int y);


    inline void setColor(Color foregroundColor, Color backgroundColor);

    void setForegroundColor(Color color);

    void setBackgroundColor(Color color);

    void resetForegroundColor();

    void resetBackgroundColor();

    void resetColor();


    void clear();



    // NOTE: For internal use only.
    class Initializer
    {
    public:

        Initializer();

        ~Initializer();

    private:
        
        static std::size_t s_counter;
    };



    // NOTE: For internal use only.
    static Initializer initializer;



    inline void writeLine(std::string_view str) {
        write(str);
        write('\n');
    }



    inline void moveCursor(int xAmount, int yAmount) {
        moveCursor({ xAmount, yAmount });
    }



    inline void setCursorPosition(int x, int y) {
        setCursorPosition({ x, y });
    }



    inline void setColor(Color foregroundColor, Color backgroundColor) {
        setForegroundColor(foregroundColor);
        setBackgroundColor(backgroundColor);
    }
}

#endif // CEDAR_TERMINAL_H