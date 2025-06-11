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
    // SizeLimits helper functions

    inline bool isLimitSet(int limit);

    bool limitsOverlap(int minLimit, int maxLimit);

    bool limitsOverlap(Cedar::Size2D<int> minSize, Cedar::Size2D<int> maxSize);

    inline Cedar::Size2D<int> clampSizeBetweenLimits(Cedar::Size2D<int> size, Cedar::Window::SizeLimits sizeLimits);



    inline bool isLimitSet(int limit) {
        return limit >= 0;
    }



    bool limitsOverlap(int minLimit, int maxLimit)
    {
        // Don't need to check if maxLimit < minLimit
        // because that's equivalent to minLimit > maxLimit.
        return (isLimitSet(minLimit) && isLimitSet(maxLimit) && minLimit > maxLimit);
    }

    bool limitsOverlap(Cedar::Size2D<int> minSize, Cedar::Size2D<int> maxSize)
    {
        return (limitsOverlap(minSize.width, maxSize.width) || limitsOverlap(minSize.height, maxSize.height));
    }



    Cedar::Size2D<int> clampSizeBetweenLimits(Cedar::Size2D<int> size, Cedar::Window::SizeLimits sizeLimits)
    {
        Cedar::Size2D<int> result;
        int min;
        int max;

        min = (isLimitSet(sizeLimits.minSize.width) ? sizeLimits.minSize.width : size.width);
        max = (isLimitSet(sizeLimits.maxSize.width) ? sizeLimits.maxSize.width : size.width);
        result.width = std::clamp(size.width, min, max);

        // TODO: Height.

        return result;
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
            m_size = clampSizeBetweenLimits(windowSize, m_sizeLimits);

        return *this;
    }



    OpenArgs& OpenArgs::sizeLimits(SizeLimits windowSizeLimits)
    {

        if (limitsOverlap(windowSizeLimits.minSize, windowSizeLimits.maxSize))
            throw std::logic_error("The window size limits overlap");

        m_sizeLimits = windowSizeLimits;

        // Set size to itself and let OpenArgs::size handle
        // checking for default values and clamping
        (void)size(m_size);

        return *this;
    }



    OpenArgs& OpenArgs::minSize(Size2D<int> windowMinSize)
    {
        if (limitsOverlap(windowMinSize, getSizeLimits().maxSize))
            throw std::logic_error("The minimum window size exceeded the maximum window size");

        m_sizeLimits.minSize = windowMinSize;

        // Set size to itself and let OpenArgs::size handle
        // checking for default values and clamping
        (void)size(m_size);

        return *this;
    }



    OpenArgs& OpenArgs::maxSize(Size2D<int> windowMaxSize)
    {
        if (limitsOverlap(getSizeLimits().minSize, windowMaxSize))
            throw std::logic_error("The maximum window size exceeded the minimum window size");

        m_sizeLimits.maxSize = windowMaxSize;

        // Set size to itself and let OpenArgs::size handle
        // checking for default values and clamping
        (void)size(m_size);

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