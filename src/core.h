//
// Macros used throughout the Cedar engine.
//

#ifndef CEDAR_CORE_H
#define CEDAR_CORE_H

#define CEDAR_FALSE 0
#define CEDAR_TRUE  1

#define CEDAR_STRINGIFY_MACRO(macro) CEDAR_STRINGIFY(macro)
#define CEDAR_STRINGIFY(x)           #x

// OS
#if defined(__linux__)
    #define CEDAR_OS_LINUX
#elif defined(_WIN64)
    #define CEDAR_OS_WINDOWS
#else
    #error Unknown or unsupported OS
#endif

// Debug
#if defined(_DEBUG) || defined(DEBUG)
    #define PINE_DEBUG
#endif

#endif // CEDAR_CORE_H