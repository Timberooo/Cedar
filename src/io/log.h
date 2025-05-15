#ifndef CEDAR_IO_LOG_H
#define CEDAR_IO_LOG_H

#include "../core.h"

#include <cstddef>
#include <string_view>

#define CEDAR_LOG_LEVEL_DEBUG    0
#define CEDAR_LOG_LEVEL_INFO     1
#define CEDAR_LOG_LEVEL_WARNING  2
#define CEDAR_LOG_LEVEL_ERROR    3
#define CEDAR_LOG_LEVEL_CRITICAL 4
#define CEDAR_LOG_LEVEL_FATAL    5

#if !defined(CEDAR_LOG_MIN_LEVEL)
    #define CEDAR_LOG_MIN_LEVEL CEDAR_LOG_LEVEL_DEBUG
#endif



namespace Cedar::Log
{
    // For internal use only
    class LogInitializer
    {
    public:

        LogInitializer();

        ~LogInitializer();

    private:

        static std::size_t s_counter;
    };



    // For internal use only
    static LogInitializer logInitializer;



    enum class Level {
        debug    = CEDAR_LOG_LEVEL_DEBUG,
        info     = CEDAR_LOG_LEVEL_INFO,
        warning  = CEDAR_LOG_LEVEL_WARNING,
        error    = CEDAR_LOG_LEVEL_ERROR,
        critical = CEDAR_LOG_LEVEL_CRITICAL,
        fatal    = CEDAR_LOG_LEVEL_FATAL
    };



    Level getMinLevel();

    void setMinLevel(Level level);


    void message(Level level, std::string_view msg);

    CEDAR_FORCE_INLINE void debug(std::string_view msg);

    CEDAR_FORCE_INLINE void info(std::string_view msg);

    CEDAR_FORCE_INLINE void warning(std::string_view msg);

    CEDAR_FORCE_INLINE void error(std::string_view msg);

    CEDAR_FORCE_INLINE void critical(std::string_view msg);

    CEDAR_FORCE_INLINE void fatal(std::string_view msg);



    CEDAR_FORCE_INLINE void debug(std::string_view msg) {
        message(Level::debug, msg);
    }



    CEDAR_FORCE_INLINE void info(std::string_view msg) {
        message(Level::info, msg);
    }



    CEDAR_FORCE_INLINE void warning(std::string_view msg) {
        message(Level::warning, msg);
    }



    CEDAR_FORCE_INLINE void error(std::string_view msg) {
        message(Level::error, msg);
    }



    CEDAR_FORCE_INLINE void critical(std::string_view msg) {
        message(Level::critical, msg);
    }



    CEDAR_FORCE_INLINE void fatal(std::string_view msg) {
        message(Level::fatal, msg);
    }
}



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