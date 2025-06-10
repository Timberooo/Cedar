#include "window.h"

#include "callback.h"
#include "core.h"
#include "io/log.h"

#include <algorithm>
#include <cstddef>
#include <new>
#include <stdexcept>
#include <type_traits>



namespace
{
    struct WindowData;
}



// OS-specific definition of WindowData
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "platform/windows.h"



namespace
{
    struct WindowData
    {
        struct Callbacks
        {
            Cedar::Callback<Cedar::Window::ClosedFunc>            closed;
            Cedar::Callback<Cedar::Window::ClosingFunc>           closing;
            Cedar::Callback<Cedar::Window::KeyPressedFunc>        keyPressed;
            Cedar::Callback<Cedar::Window::ResizedFunc>           resized;
            Cedar::Callback<Cedar::Window::VisibilityChangedFunc> visibilityChanged;
        } callback;

        HWND window = NULL;


        inline WindowData() {}

        inline ~WindowData() {}
    };
}

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



// OS-agnostic implementation
namespace
{
    inline bool isSizeLimitSet(int value);



    inline bool isSizeLimitSet(int value) {
        return value >= 0;
    }
}



namespace Cedar::Window
{
    OpenArgs& OpenArgs::position(Point2D<int> windowPosition)
    {
        if (windowPosition.x = defaultPosition.x || windowPosition.y == defaultPosition.y)
            m_position = defaultPosition;
        else
            m_position = windowPosition;

        return *this;
    }



    OpenArgs& OpenArgs::size(Size2D<int> windowSize)
    {
        if (windowSize.width < 0 || windowSize.height < 0)
            m_size = defaultSize;
        else
        {
            // Clamp width
            if (getSizeLimits().minSize.width >= 0)
                m_size.width = std::max(windowSize.width, getSizeLimits().minSize.width);
            if (getSizeLimits().maxSize.width >= 0)
                m_size.width = std::min(m_size.width, getSizeLimits().maxSize.width);

            // Clamp height
            if (getSizeLimits().minSize.height >= 0)
                m_size.height = std::max(windowSize.height, getSizeLimits().minSize.height);
            if (getSizeLimits().maxSize.height >= 0)
                m_size.height = std::min(m_size.height, getSizeLimits().maxSize.height);
        }

        return *this;
    }



    OpenArgs& OpenArgs::sizeLimits(SizeLimits windowSizeLimits)
    {
        minSize(windowSizeLimits.minSize);
        maxSize(windowSizeLimits.maxSize);

        return *this;
    }



    OpenArgs& OpenArgs::minSize(Size2D<int> windowMinSize)
    {
        if ((isSizeLimitSet(windowMinSize.width) && isSizeLimitSet(getSizeLimits().maxSize.width) && windowMinSize.width > getSizeLimits().maxSize.width) ||
            (isSizeLimitSet(windowMinSize.height) && isSizeLimitSet(getSizeLimits().maxSize.height) && windowMinSize.height > getSizeLimits().maxSize.height))
            throw std::logic_error("The minimum window size exceeded the maximum window size");
        else
            m_sizeLimits.minSize = windowMinSize;

        // Setting m_size to itself so the size function can handle clamping the size.
        size(getSize());

        return *this;
    }



    OpenArgs& OpenArgs::maxSize(Size2D<int> windowMaxSize)
    {
        if ((isSizeLimitSet(windowMaxSize.width) && isSizeLimitSet(getSizeLimits().minSize.width) && windowMaxSize.width < getSizeLimits().minSize.width) ||
            (isSizeLimitSet(windowMaxSize.height) && isSizeLimitSet(getSizeLimits().minSize.height) && windowMaxSize.height < getSizeLimits().minSize.height))
            throw std::logic_error("The maximum window size exceeded the minimum window size");
        else
            m_sizeLimits.maxSize = windowMaxSize;

        // Setting m_size to itself so the size function can handle clamping the size.
        size(getSize());

        return *this;
    }
}
// OS-agnostic implementation



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

namespace Cedar::Window
{
    void open(const OpenArgs& openArgs)
    {
        // TODO: Implement this function.
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// URGENT: Implement Linux version.

#endif // ^^^ Linux ^^^
// OS-specific implementation