//
// Logging utilities for writing messages to the terminal (if one is visible).
// 
// Support for logging to a file is planned.
//

#ifndef CEDAR_IO_LOG_H
#define CEDAR_IO_LOG_H

#include "../core.h"

#include <cstddef>
#include <string_view>

#define CEDAR_LOG_LEVEL_TRACE    0
#define CEDAR_LOG_LEVEL_DEBUG    1
#define CEDAR_LOG_LEVEL_INFO     2
#define CEDAR_LOG_LEVEL_WARNING  3
#define CEDAR_LOG_LEVEL_ERROR    4
#define CEDAR_LOG_LEVEL_CRITICAL 5
#define CEDAR_LOG_LEVEL_FATAL    6

#if !defined(CEDAR_LOG_MIN_LEVEL)
    #define CEDAR_LOG_MIN_LEVEL CEDAR_LOG_LEVEL_TRACE
#endif



namespace Cedar::Log
{
    // Nifty counter. For internal use only
    class LogInitializer
    {
    public:

        LogInitializer();

        ~LogInitializer();

    private:

        static std::size_t s_counter;
    };



    // Nifty counter. For internal use only
    static LogInitializer logInitializer;



    enum class Level {
        Trace    = CEDAR_LOG_LEVEL_TRACE,
        Debug    = CEDAR_LOG_LEVEL_DEBUG,
        Info     = CEDAR_LOG_LEVEL_INFO,
        Warning  = CEDAR_LOG_LEVEL_WARNING,
        Error    = CEDAR_LOG_LEVEL_ERROR,
        Critical = CEDAR_LOG_LEVEL_CRITICAL,
        Fatal    = CEDAR_LOG_LEVEL_FATAL,
    };



    Level getMinLevel();

    void setMinLevel(Level level);


    void message(Level level, std::string_view msg);

    CEDAR_FORCE_INLINE void trace(std::string_view msg);

    CEDAR_FORCE_INLINE void debug(std::string_view msg);

    CEDAR_FORCE_INLINE void info(std::string_view msg);

    CEDAR_FORCE_INLINE void warning(std::string_view msg);

    CEDAR_FORCE_INLINE void error(std::string_view msg);

    CEDAR_FORCE_INLINE void critical(std::string_view msg);

    CEDAR_FORCE_INLINE void fatal(std::string_view msg);



    CEDAR_FORCE_INLINE void trace(std::string_view msg) {
        message(Level::Trace, msg);
    }



    CEDAR_FORCE_INLINE void debug(std::string_view msg) {
        message(Level::Debug, msg);
    }



    CEDAR_FORCE_INLINE void info(std::string_view msg) {
        message(Level::Info, msg);
    }



    CEDAR_FORCE_INLINE void warning(std::string_view msg) {
        message(Level::Warning, msg);
    }



    CEDAR_FORCE_INLINE void error(std::string_view msg) {
        message(Level::Error, msg);
    }



    CEDAR_FORCE_INLINE void critical(std::string_view msg) {
        message(Level::Critical, msg);
    }



    CEDAR_FORCE_INLINE void fatal(std::string_view msg) {
        message(Level::Fatal, msg);
    }
}



#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_TRACE
    #define CEDAR_LOG_TRACE(msg) Cedar::Log::trace(msg)
#else
    #define CEDAR_LOG_TRACE(msg) ((void)0)
#endif

#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_DEBUG
    #define CEDAR_LOG_DEBUG(msg) Cedar::Log::debug(msg)
#else
    #define CEDAR_LOG_DEBUG(msg) ((void)0)
#endif

#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_INFO
    #define CEDAR_LOG_INFO(msg) Cedar::Log::info(msg)
#else
    #define CEDAR_LOG_INFO(msg) ((void)0)
#endif

#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_WARNING
    #define CEDAR_LOG_WARNING(msg) Cedar::Log::warning(msg)
#else
    #define CEDAR_LOG_WARNING(msg) ((void)0)
#endif

#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_ERROR
    #define CEDAR_LOG_ERROR(msg) Cedar::Log::error(msg)
#else
    #define CEDAR_LOG_ERROR(msg) ((void)0)
#endif

#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_CRITICAL
    #define CEDAR_LOG_CRITICAL(msg) Cedar::Log::critical(msg)
#else
    #define CEDAR_LOG_CRITICAL(msg) ((void)0)
#endif

#if CEDAR_LOG_MIN_LEVEL <= CEDAR_LOG_LEVEL_FATAL
    #define CEDAR_LOG_FATAL(msg) Cedar::Log::fatal(msg)
#else
    #define CEDAR_LOG_FATAL(msg) ((void)0)
#endif

#endif // CEDAR_IO_LOG_H