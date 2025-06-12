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
    struct WindowStyles;



    struct WindowStyles
    {
        DWORD style;
        DWORD exStyle;
    };



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

        HWND hWnd        = NULL;
        ATOM windowClass = 0;

        Cedar::Window::SizeLimits sizeLimits = { { -1, -1 }, { -1, -1 } };
        WindowStyles              styles     = { 0, 0 };


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
        Cedar::Size2D<int> result = size;

        // Clamp width
        if (isLimitSet(sizeLimits.minSize.width))
            result.width = std::max(result.width, sizeLimits.minSize.width);
        if (isLimitSet(sizeLimits.maxSize.width))
            result.width = std::min(result.width, sizeLimits.maxSize.width);

        // Clamp height
        if (isLimitSet(sizeLimits.minSize.height))
            result.height = std::max(result.height, sizeLimits.minSize.height);
        if (isLimitSet(sizeLimits.maxSize.height))
            result.height = std::min(result.height, sizeLimits.maxSize.height);

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
        {
            m_size = clampSizeBetweenLimits(windowSize, m_sizeLimits);
            Cedar::Log::warning("Window size exceeded the size limits and was clamped");
        }

        return *this;
    }



    OpenArgs& OpenArgs::sizeLimits(SizeLimits windowSizeLimits)
    {

        if (limitsOverlap(windowSizeLimits.minSize, windowSizeLimits.maxSize))
            throw std::logic_error("Window size limits overlap");

        m_sizeLimits = windowSizeLimits;

        // Set size to itself and let OpenArgs::size handle
        // checking for default values and clamping
        (void)size(m_size);

        return *this;
    }



    OpenArgs& OpenArgs::minSize(Size2D<int> windowMinSize)
    {
        if (limitsOverlap(windowMinSize, getSizeLimits().maxSize))
            throw std::logic_error("Minimum window size exceeded the maximum window size");

        m_sizeLimits.minSize = windowMinSize;

        // Set size to itself and let OpenArgs::size handle
        // checking for default values and clamping
        (void)size(m_size);

        return *this;
    }



    OpenArgs& OpenArgs::maxSize(Size2D<int> windowMaxSize)
    {
        if (limitsOverlap(getSizeLimits().minSize, windowMaxSize))
            throw std::logic_error("Maximum window size exceeded the minimum window size");

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

#include "platform/windows.h"

#include <system_error>



namespace
{
    constexpr LPCWSTR windowClassName = L"CedarEngine";



    void registerWindowClass();

    WindowStyles getWindowStyles();

    Cedar::Size2D<int> clientSizeToWindowSize(Cedar::Size2D<int> clientSize, WindowStyles styles, UINT dpi);

    int visibilityToCmdShow(Cedar::Window::Visibility vis);

    Cedar::Window::Visibility cmdShowToVisibility(int cmdShow);



    LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT wmGetMinMaxInfo(HWND hWnd, LPARAM lParam);

    LRESULT wmSize(HWND hWnd, WPARAM wParam, LPARAM lParam);

    LRESULT wmClose(HWND hWnd);

    LRESULT wmDestroy(HWND hWnd);



    void registerWindowClass()
    {
        if (g_windowData.windowClass != 0)
        {
            CEDAR_LOG_TRACE("Window class already registered");
            return;
        }

        WNDCLASSEXW wndClass;
        ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
        wndClass.cbSize        = sizeof(WNDCLASSEXW);
        wndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndClass.lpfnWndProc   = windowProc;
        wndClass.hInstance     = Cedar::Platform::Windows::getInstance();
        wndClass.lpszClassName = windowClassName;

        g_windowData.windowClass = RegisterClassExW(&wndClass);

        if (g_windowData.windowClass == 0)
            throw std::system_error(GetLastError(), std::system_category(),
                                    "Failed to register window class");

        CEDAR_LOG_DEBUG("Registered window class");
    }



    WindowStyles getWindowStyles()
    {
        WindowStyles styles;

        styles.style   = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        styles.exStyle = 0;

        // TODO: Check for window modes (windowed, fullscreen, and borderless) and assign
        //       styles based on the mode.
        styles.style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

        styles.style |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;

        return styles;
    }



    Cedar::Size2D<int> clientSizeToWindowSize(Cedar::Size2D<int> clientSize, WindowStyles styles, UINT dpi)
    {
        RECT rect = { 0, 0, clientSize.width, clientSize.height };
        AdjustWindowRectExForDpi(&rect, styles.style, FALSE, styles.exStyle, dpi);

        return { rect.right - rect.left, rect.bottom - rect.top };
    }



    int visibilityToCmdShow(Cedar::Window::Visibility vis)
    {
        switch (vis) {
            case Cedar::Window::Visibility::Show:
                return SW_SHOW;
            case Cedar::Window::Visibility::Hide:
                return SW_HIDE;
            case Cedar::Window::Visibility::Maximize:
                return SW_MAXIMIZE;
            default: // Cedar::Window::Visibility::Minimize
                return SW_MINIMIZE;
        }
    }



    Cedar::Window::Visibility cmdShowToVisibility(int cmdShow)
    {
        switch (cmdShow) {
            case SW_SHOW:
                return Cedar::Window::Visibility::Show;
            case SW_HIDE:
                return Cedar::Window::Visibility::Hide;
            case SW_MAXIMIZE:
                return Cedar::Window::Visibility::Maximize;
            default: // SW_MINIMIZE
                return Cedar::Window::Visibility::Minimize;
        }
    }



    LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
            case WM_GETMINMAXINFO:
                return wmGetMinMaxInfo(hWnd, lParam);
            case WM_SIZE:
                return wmSize(hWnd, wParam, lParam);
            case WM_CLOSE:
                return wmClose(hWnd);
            case WM_DESTROY:
                return wmDestroy(hWnd);
            default:
                return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        }
    }



    LRESULT wmGetMinMaxInfo(HWND hWnd, LPARAM lParam)
    {
        // Check if window is open because this event can happen during window creation.
        if (Cedar::Window::isOpen())
            return 0;

        Cedar::Window::SizeLimits sizeLimits;
        sizeLimits.minSize = clientSizeToWindowSize(g_windowData.sizeLimits.minSize, g_windowData.styles, GetDpiForWindow(hWnd));
        sizeLimits.maxSize = clientSizeToWindowSize(g_windowData.sizeLimits.maxSize, g_windowData.styles, GetDpiForWindow(hWnd));

        // Limit window size if limits are set
        if (isLimitSet(g_windowData.sizeLimits.minSize.width))
            (*(LPMINMAXINFO)lParam).ptMinTrackSize.x = sizeLimits.minSize.width;
        if (isLimitSet(g_windowData.sizeLimits.minSize.height))
            (*(LPMINMAXINFO)lParam).ptMinTrackSize.y = sizeLimits.minSize.height;
        if (isLimitSet(g_windowData.sizeLimits.maxSize.width))
            (*(LPMINMAXINFO)lParam).ptMaxTrackSize.x = sizeLimits.maxSize.width;
        if (isLimitSet(g_windowData.sizeLimits.maxSize.height))
            (*(LPMINMAXINFO)lParam).ptMaxTrackSize.y = sizeLimits.maxSize.height;

        return 0;
    }



    LRESULT wmSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        if (wParam != SIZE_MINIMIZED)
            (void)g_windowData.callback.resized.tryCall();

        return 0;
    }



    LRESULT wmClose(HWND hWnd)
    {
        bool close = true;

        if (g_windowData.callback.closing.canCall())
            close = g_windowData.callback.closing.call();

        if (close)
            DestroyWindow(hWnd);

        return 0;
    }



    LRESULT wmDestroy(HWND hWnd)
    {
        (void)g_windowData.callback.closed.tryCall();

        g_windowData.hWnd = NULL;
        PostQuitMessage(0);

        return 0;
    }
}



namespace Cedar::Window
{
    bool isOpen()
    {
        return g_windowData.hWnd != NULL;
    }



    void open(const OpenArgs& openArgs)
    {
        registerWindowClass();

        WindowStyles styles = getWindowStyles();

        Point2D<int> pos;
        Size2D<int> size;

        if (openArgs.getPosition() == OpenArgs::defaultPosition)
            pos = Point2D<int>(CW_USEDEFAULT, CW_USEDEFAULT);
        else
        {
            pos = openArgs.getPosition();

            // NOTE: Adjusting x and y if they happen to equal CW_USEDEFAULT to avoid the
            //       window appearing at the default position. This will likely never
            //       happen given what the value of CW_USEDEFAULT is, but it should still
            //       be addressed.
            if (pos.x == CW_USEDEFAULT)
                pos.x++;
            if (pos.y == CW_USEDEFAULT)
                pos.y++;
        }

        if (openArgs.getSize() == OpenArgs::defaultSize)
            size = Size2D<int>(CW_USEDEFAULT, CW_USEDEFAULT);
        else
            size = clientSizeToWindowSize(openArgs.getSize(), styles, USER_DEFAULT_SCREEN_DPI);

        g_windowData.hWnd = CreateWindowExW(styles.exStyle,
                                            MAKEINTATOM(g_windowData.windowClass),
                                            Platform::Windows::stringToWideString(openArgs.getTitle()).c_str(),
                                            styles.style,
                                            pos.x, pos.y,
                                            size.width, size.height,
                                            NULL, NULL,
                                            Platform::Windows::getInstance(),
                                            NULL);

        if (g_windowData.hWnd == NULL)
            throw std::system_error(GetLastError(), std::system_category(),
                                    "Failed to create window");

        g_windowData.sizeLimits = openArgs.getSizeLimits();
        g_windowData.styles     = styles;

        ShowWindow(g_windowData.hWnd, visibilityToCmdShow(openArgs.getVisibility()));
    }



    void close()
    {
        PostMessage(g_windowData.hWnd, WM_CLOSE, 0, 0);
    }



    void pollEvents()
    {
        MSG msg;

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message != WM_QUIT)
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            else
            {
                UnregisterClassW(MAKEINTATOM(g_windowData.windowClass),
                                 Platform::Windows::getInstance());
            }
        }
    }



    void setClosedCallback(ClosedFunc closedCallback)
    {
        g_windowData.callback.closed.set(closedCallback);
    }



    void setClosingCallback(ClosingFunc closingCallback)
    {
        g_windowData.callback.closing.set(closingCallback);
    }



    void setResizedCallback(ResizedFunc resizedCallback)
    {
        g_windowData.callback.resized.set(resizedCallback);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// URGENT: Implement Linux version.

#endif // ^^^ Linux ^^^
// OS-specific implementation