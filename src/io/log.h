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

#endif // CEDAR_IO_LOG_H