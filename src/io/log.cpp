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
        Cedar::Log::Level minLevel = Cedar::Log::Level::Trace;
    #else
        Cedar::Log::Level minLevel = Cedar::Log::Level::Info;
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
            case Cedar::Log::Level::Trace:
                return "TRACE";
            case Cedar::Log::Level::Debug:
                return "DEBUG";
            case Cedar::Log::Level::Info:
                return "INFO ";
            case Cedar::Log::Level::Warning:
                return "WARN ";
            case Cedar::Log::Level::Error:
                return "ERROR";
            case Cedar::Log::Level::Critical:
                return "CRIT ";
            case Cedar::Log::Level::Fatal:
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

        Terminal::Color foregroundColor = Terminal::Color::Use_Default;
        Terminal::Color backgroundColor = Terminal::Color::Use_Default;
        
        switch (level) {
            case Level::Trace:
                foregroundColor = Terminal::Color::White; break;
            case Level::Debug:
                foregroundColor = Terminal::Color::Bright_Magenta; break;
            case Level::Info:
                foregroundColor = Terminal::Color::Green; break;
            case Level::Warning:
                foregroundColor = Terminal::Color::Bright_Yellow; break;
            case Level::Error:
                foregroundColor = Terminal::Color::Red; break;
            default: { // Level::Critical and Level::Fatal
                foregroundColor = Terminal::Color::White;
                backgroundColor = Terminal::Color::Red;
                break;
            }
        }

        Terminal::write(getPrefix(level), foregroundColor, backgroundColor);
        Terminal::writeLine(msg, foregroundColor, backgroundColor);
    }
}