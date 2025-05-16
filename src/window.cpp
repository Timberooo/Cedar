#include "window.h"

#include "core.h"

#include <cstddef>
#include <new>
#include <type_traits>



namespace
{
    struct WindowInternals;
}



// OS-specific implementation of WindowInternals
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

namespace
{
    struct WindowInternals
    {
        Cedar::WindowClosedFunc  windowClosedCallback  = nullptr;
        Cedar::WindowClosingFunc windowClosingCallback = nullptr;
        Cedar::WindowResizedFunc windowResizedCallback = nullptr;


        WindowInternals();

        ~WindowInternals();
    };
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// TODO: Implement Linux version.

#endif // ^^^ Linux ^^^



namespace
{
    static typename std::aligned_storage<sizeof(Cedar::WindowInitializer), alignof(Cedar::WindowInitializer)>::type g_windowInternalsBuff;

    WindowInternals& g_windowInternals = reinterpret_cast<WindowInternals&>(g_windowInternalsBuff);
}



namespace Cedar
{
    std::size_t WindowInitializer::s_counter = 0;



    WindowInitializer::WindowInitializer()
    {
        if (s_counter == 0)
            new (&g_windowInternals)WindowInternals();

        s_counter++;
    }



    WindowInitializer::~WindowInitializer()
    {
        s_counter--;

        if (s_counter == 0)
            g_windowInternals.~WindowInternals();
    }
}



// OS-specific implementation of window.h
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

namespace Cedar
{

}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// TODO: Implement Linux version.

#endif // ^^^ Linux ^^^