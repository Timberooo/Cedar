#include "window.h"

#include "callback.h"
#include "core.h"
#include "io/log.h"

#include <cstddef>
#include <new>
#include <type_traits>



namespace
{
    struct WindowData;
}



// OS-specific definition of WindowData
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

// URGENT: Implement Windows version.

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

namespace
{
    struct WindowData
    {
        // TODO: Add necessary members.
    };
}

#endif // ^^^ Linux ^^^
// OS-specific definition of WindowData



// Nifty counter internal details
namespace
{
    static typename std::aligned_storage<sizeof(WindowData), alignof(WindowData)>::type g_windowDataBuffer;

    WindowData& g_windowData = reinterpret_cast<WindowData&>(g_windowDataBuffer);
}



namespace Cedar::Window
{
    std::size_t WindowInitializer::s_counter = 0;



    WindowInitializer::WindowInitializer()
    {
        if (s_counter == 0)
            new(&g_windowData)WindowData();

        s_counter++;
    }



    WindowInitializer::~WindowInitializer()
    {
        s_counter--;

        if (s_counter == 0)
            g_windowData.~WindowData();
    }
}
// Nifty counter internal details



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

// URGENT: Implement Windows version.

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// URGENT: Implement Linux version.

#endif // ^^^ Linux ^^^
// OS-specific implementation