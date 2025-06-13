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
    #define CEDAR_DEBUG
#endif

// Compiler
#if defined(_MSC_VER)
    #define CEDAR_COMPILER_MSVC _MSC_VER
#endif

// Force inline
#if defined(CEDAR_COMPILER_MSVC)
    #define CEDAR_FORCE_INLINE __forceinline
    #define CEDAR_FORCE_INLINE_SUPPORTED CEDAR_TRUE
#else
    // If compiler doesn't support forced inling, fallback to regular inlining
    #define CEDAR_FORCE_INLINE inline
    #define CEDAR_FORCE_INLINE_SUPPORTED CEDAR_FALSE
#endif

#endif // CEDAR_CORE_H