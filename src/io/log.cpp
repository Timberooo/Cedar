#include "log.h"

#include "terminal.h"
#include "../core.h"

#include <chrono>
#include <cstddef>
#include <format>
#include <new>
#include <string>
#include <string_view>
#include <type_traits>



namespace
{
    struct LogData;



    struct LogData
    {
    #if defined(CEDAR_DEBUG)
        Cedar::Log::Level minLevel = Cedar::Log::Level::trace;
    #else
        Cedar::Log::Level minLevel = Cedar::Log::Level::info;
    #endif


        inline LogData() {}

        inline ~LogData() {}
    };



    std::string getPrefix(Cedar::Log::Level level);

    std::string getTimestamp();

    std::string getLevelName(Cedar::Log::Level level);



    std::string getPrefix(Cedar::Log::Level level)
    {
        return '[' + getTimestamp() + " UTC][" + getLevelName(level) + "]: ";
    }



    std::string getTimestamp()
    {
        using namespace std::chrono;

        auto utcTime = utc_clock::from_sys(time_point_cast<seconds>(system_clock::now()));

        return std::format("{0:%T}", utcTime);
    }



    std::string getLevelName(Cedar::Log::Level level)
    {
        switch (level) {
            case Cedar::Log::Level::trace:
                return "TRACE";
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
            default: // Should never be hit but this shuts the compiler up
                return "LEVEL";
        }
    }
}



// Nifty counter internal details
namespace
{
    static typename std::aligned_storage<sizeof(LogData), alignof(LogData)>::type g_logDataBuffer;

    LogData& g_logData = reinterpret_cast<LogData&>(g_logDataBuffer);
}



namespace Cedar::Log
{
    std::size_t LogInitializer::s_counter = 0;



    LogInitializer::LogInitializer()
    {
        if (s_counter == 0)
            new (&g_logData)LogData();

        s_counter++;
    }



    LogInitializer::~LogInitializer()
    {
        s_counter--;

        if (s_counter == 0)
            g_logData.~LogData();
    }
}
// Nifty counter internal details



namespace Cedar::Log
{
    Level getMinLevel()
    {
        return g_logData.minLevel;
    }



    void setMinLevel(Level level)
    {
        g_logData.minLevel = level;
    }



    void message(Level level, std::string_view msg)
    {
        if (level < getMinLevel())
            return;

        switch (level) {
            case Level::trace:
                Terminal::setForegroundColor(Terminal::Color::bright_black); break;
            case Level::debug:
                Terminal::setForegroundColor(Terminal::Color::bright_black); break;
            case Level::info:
                Terminal::setForegroundColor(Terminal::Color::white); break;
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