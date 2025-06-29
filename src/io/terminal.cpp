#include "terminal.h"

#include "../core.h"

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
    constexpr DWORD vtProcessingOutputModeFlags = ENABLE_PROCESSED_OUTPUT |
                                                  ENABLE_VIRTUAL_TERMINAL_PROCESSING;



    struct TerminalData
    {
    public:
        
        inline TerminalData();

        inline ~TerminalData();

    private:

        DWORD m_originalOutputMode = 0;
    };



    inline TerminalData::TerminalData()
    {
        (void)AttachConsole(ATTACH_PARENT_PROCESS);
        (void)GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &m_originalOutputMode);
        (void)SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), m_originalOutputMode | vtProcessingOutputModeFlags);
    }



    inline TerminalData::~TerminalData() {
        (void)SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), m_originalOutputMode);
        (void)FreeConsole();
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
    enum class ColorType : int {
        Foreground = 0,
        Background = 10
    };



    void writeInternal(std::string_view str);

    void writeInternal(char character);


    void setColor(Cedar::Terminal::Color color, ColorType type);

    void setColors(Cedar::Terminal::Color foregroundColor, Cedar::Terminal::Color backgroundColor);

    void resetColors();



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
// OS-agnostic implementation



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "../platform/windows.h"



namespace
{
    void writeInternal(std::string_view str)
    {
        (void)WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.data(), str.length(), NULL, NULL);
    }

    void writeInternal(char character)
    {
        (void)WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &character, 1, NULL, NULL);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

#include <unistd.h>



namespace
{

    void writeInternal(std::string_view str)
    {
        (void)::write(STDOUT_FILENO, str.data(), str.length());
    }

    void writeInternal(char character)
    {
        (void)::write(STDOUT_FILENO, &character, 1);
    }
}

#endif // ^^^ Linux ^^^
// OS-specific implementation