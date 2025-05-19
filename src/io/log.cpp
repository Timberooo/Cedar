#include "log.h"

#include "terminal.h"

#include <ctime>
#include <new>
#include <string>
#include <string_view>
#include <type_traits>



namespace
{
    struct LogInternals;



    struct LogInternals
    {
        inline LogInternals() {}

        inline ~LogInternals() {}


        Cedar::Log::Level minLevel = Cedar::Log::Level::debug;
    };



    static typename std::aligned_storage<sizeof(LogInternals), alignof(LogInternals)>::type g_logInternalsBuff;

    LogInternals& g_logInternals = reinterpret_cast<LogInternals&>(g_logInternalsBuff);



    std::string getPrefix(Cedar::Log::Level level);

    std::string getTimestamp();

    std::string getLevelName(Cedar::Log::Level level);



    std::string getPrefix(Cedar::Log::Level level)
    {
        return '[' + getTimestamp() + " UTC][" + getLevelName(level) + "]: ";
    }



    std::string getTimestamp()
    {
        char timestamp[9] = { '\0' };

        // URGENT: Either find a cross-platform way to implement this function or have
        //         OS-specific implementations. gmtime_s is a windows-specific function.
        std::time_t timeVal = std::time(nullptr);
        std::tm timeStruct;
        gmtime_s(&timeStruct, &timeVal);

        std::strftime(timestamp, 9, "%T", &timeStruct);

        return timestamp;
    }



    std::string getLevelName(Cedar::Log::Level level)
    {
        switch (level) {
            case Cedar::Log::Level::debug:
                return "DEBUG";
            case Cedar::Log::Level::info:
                return "INFO ";
            case Cedar::Log::Level::warning:
                return "WARN ";
            case Cedar::Log::Level::error:
                return "ERROR";
            case Cedar::Log::Level::critical:
                return "CRIT ";
            case Cedar::Log::Level::fatal:
                return "FATAL";
            default:
                return "LEVEL";
        }
    }
}



namespace Cedar::Log
{
    std::size_t LogInitializer::s_counter = 0;



    LogInitializer::LogInitializer()
    {
        if (s_counter == 0)
            new (&g_logInternals)LogInternals();

        s_counter++;
    }



    LogInitializer::~LogInitializer()
    {
        s_counter--;

        if (s_counter == 0)
            g_logInternals.~LogInternals();
    }



    Level getMinLevel()
    {
        return g_logInternals.minLevel;
    }



    void setMinLevel(Level level)
    {
        g_logInternals.minLevel = level;
    }



    void message(Level level, std::string_view msg)
    {
        if (level < getMinLevel())
            return;

        switch (level) {
            case Level::debug:
                Terminal::setForegroundColor(Terminal::Color::bright_black); break;
            case Level::info:
                Terminal::setForegroundColor(Terminal::Color::green); break;
            case Level::warning:
                Terminal::setForegroundColor(Terminal::Color::yellow); break;
            case Level::error:
                Terminal::setForegroundColor(Terminal::Color::red); break;
            default: // Level::critical and Level::fatal
                Terminal::setColor(Terminal::Color::white, Terminal::Color::red); break;
        }

        Terminal::write(getPrefix(level));
        Terminal::writeLine(msg);

        Terminal::resetColor();
    }
}