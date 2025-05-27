#include "window.h"

#include "core.h"
#include "io/log.h"
#include "math.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <limits>
#include <new>
#include <string_view>
#include <string>
#include <system_error>
#include <type_traits>
#include <unordered_map>



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
    struct ActiveWindow;
    struct WindowInstance;



    struct WindowStyles
    {
        DWORD style;
        DWORD exStyle;
    };



    struct ActiveWindow
    {
        Cedar::Window window;

        Cedar::Window::SizeLimits sizeLimits;
        WindowStyles              styles;
    };



    struct WindowInstance
    {
        HWND        hWnd;
        std::size_t instanceCount;
    };



    struct WindowData
    {
        std::unordered_map<HWND, ActiveWindow> activeWindows;

        Cedar::WindowClosedFunc  windowClosedCallback  = nullptr;
        Cedar::WindowClosingFunc windowClosingCallback = nullptr;
        Cedar::WindowResizedFunc windowResizedCallback = nullptr;

        ATOM windowClass = 0;


        inline WindowData() {}

        inline ~WindowData() {}
    };
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// TODO: Implement Linux version.

#endif // ^^^ Linux ^^^
// OS-specific definition of WindowData



// Nifty counter internal details
namespace
{
    static typename std::aligned_storage<sizeof(WindowData), alignof(WindowData)>::type g_windowDataBuffer;

    WindowData& g_windowData = reinterpret_cast<WindowData&>(g_windowDataBuffer);
}



namespace Cedar
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
namespace Cedar
{
    Window::Builder& Window::Builder::size(Size2D<int> winSize)
    {
        if (winSize.width < 0)
            m_size.width = -1;
        else
            m_size.width = std::clamp(winSize.width, m_sizeLimits.minSize.width, m_sizeLimits.maxSize.width);

        if (winSize.height < 0)
            m_size.height = -1;
        else
            m_size.height = std::clamp(winSize.height, m_sizeLimits.minSize.height, m_sizeLimits.minSize.height);

        return *this;
    }



    Window::Builder& Window::Builder::maxSize(Size2D<int> winMaxSize)
    {
        m_sizeLimits.maxSize = winMaxSize;

        // Reduce m_minSize if the width or height exceed the new max size
        m_sizeLimits.minSize.width = std::min(m_sizeLimits.minSize.width, m_sizeLimits.maxSize.width);
        m_sizeLimits.minSize.height = std::min(m_sizeLimits.minSize.height, m_sizeLimits.maxSize.height);

        // Let size handle limiting m_size using the new max and min values
        size(m_size);

        return *this;
    }



    Window::Builder& Window::Builder::minSize(Size2D<int> winMinSize)
    {
        m_sizeLimits.minSize = winMinSize;

        // Increase m_maxSize if the width or height exceed the new min size
        m_sizeLimits.maxSize.width = std::max(m_sizeLimits.maxSize.width, m_sizeLimits.minSize.width);
        m_sizeLimits.maxSize.height = std::max(m_sizeLimits.maxSize.height, m_sizeLimits.minSize.height);

        // Let size handle limiting m_size using the new max and min values
        size(m_size);

        return *this;
    }
}
// OS-agnostic implementation



// OS-specific implementation
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

#include "platform/windows.h"



namespace
{
    void registerWindowClass();

    WindowStyles getWindowStyles();

    Cedar::Size2D<int> clientSizeToWindowSize(Cedar::Size2D<int> clientSize, WindowStyles styles, UINT dpi);


    int visibilityToCmdShow(Cedar::Window::Visibility vis);

    Cedar::Window::Visibility cmdShowToVisibility(int cmdShow);


    LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline LRESULT wmGetMinMaxInfo(HWND hWnd, LPARAM lParam);

    inline LRESULT wmSize(HWND hWnd, WPARAM wParam, LPARAM lParam);

    inline LRESULT wmClose(HWND hWnd);

    inline LRESULT wmDestroy(HWND hWnd);



    void registerWindowClass()
    {
        if (g_windowData.windowClass != 0)
        {
            Cedar::Log::trace("Window class already registered");
            return;
        }

        WNDCLASSEXW wndClass;
        ZeroMemory(&wndClass, sizeof(WNDCLASSEXW));
        wndClass.cbSize        = sizeof(WNDCLASSEXW);
        wndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndClass.lpfnWndProc   = windowProc;
        wndClass.hInstance     = Cedar::Platform::Windows::getInstance();
        wndClass.lpszClassName = L"CedarEngine";

        // Should remove once OpenGL rendering is implemented
        wndClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

        g_windowData.windowClass = RegisterClassExW(&wndClass);

        if (g_windowData.windowClass != 0)
            Cedar::Log::debug("Registered window class");
        else
            throw std::system_error(GetLastError(), std::system_category(),
                                    "Failed to register window class");
    }



    WindowStyles getWindowStyles()
    {
        WindowStyles styles;

        styles.style   = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        styles.exStyle = 0;

        // TODO: Check for window modes (windowed, fullscreen, borderless) and assign
        //       styles based on modes.

        styles.style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

        // TODO: Check if each of these should be added.
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
            case Cedar::Window::Visibility::show:
                return SW_SHOW;
            case Cedar::Window::Visibility::hide:
                return SW_HIDE;
            case Cedar::Window::Visibility::maximize:
                return SW_MAXIMIZE;
            default: // Cedar::Winow::Visibility::minimize
                return SW_MINIMIZE;
        }
    }



    Cedar::Window::Visibility cmdShowToVisibility(int cmdShow)
    {
        switch (cmdShow) {
            case SW_SHOW:
                return Cedar::Window::Visibility::show;
            case SW_HIDE:
                return Cedar::Window::Visibility::hide;
            case SW_MAXIMIZE:
                return Cedar::Window::Visibility::maximize;
            default: // SW_MINIMIZE
                return Cedar::Window::Visibility::minimize;
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



    inline LRESULT wmGetMinMaxInfo(HWND hWnd, LPARAM lParam)
    {
        // Check if window is active because this event can be triggered during window
        // creation before its added to the active windows
        if (!g_windowData.activeWindows.contains(hWnd))
            return 0;

        ActiveWindow window = g_windowData.activeWindows.at(hWnd);

        Cedar::Window::SizeLimits sizeLimits;
        sizeLimits.maxSize = clientSizeToWindowSize(window.sizeLimits.maxSize, window.styles, GetDpiForWindow(hWnd));
        sizeLimits.minSize = clientSizeToWindowSize(window.sizeLimits.minSize, window.styles, GetDpiForWindow(hWnd));

        // Limit window size if limits are set (>= 0)
        if (window.sizeLimits.maxSize.width >= 0)
            (*(LPMINMAXINFO)lParam).ptMaxTrackSize.x = sizeLimits.maxSize.width;
        if (window.sizeLimits.maxSize.height >= 0)
            (*(LPMINMAXINFO)lParam).ptMaxTrackSize.y = sizeLimits.maxSize.height;
        if (window.sizeLimits.minSize.width >= 0)
            (*(LPMINMAXINFO)lParam).ptMinTrackSize.x = sizeLimits.minSize.width;
        if (window.sizeLimits.minSize.height >= 0)
            (*(LPMINMAXINFO)lParam).ptMinTrackSize.y = sizeLimits.minSize.height;

        return 0;
    }



    inline LRESULT wmSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
    {
        if (g_windowData.windowResizedCallback && wParam != SIZE_MINIMIZED)
            g_windowData.windowResizedCallback(g_windowData.activeWindows.at(hWnd).window);

        return 0;
    }



    inline LRESULT wmClose(HWND hWnd)
    {
        bool close = true;

        if (g_windowData.windowClosingCallback)
            close = g_windowData.windowClosingCallback(g_windowData.activeWindows.at(hWnd).window);

        if (close)
            DestroyWindow(hWnd);

        return 0;
    }



    inline LRESULT wmDestroy(HWND hWnd)
    {
        if (g_windowData.windowClosedCallback)
            g_windowData.windowClosedCallback(g_windowData.activeWindows.at(hWnd).window);

        g_windowData.activeWindows.erase(hWnd);

        // TODO: Clear the handle for this window's instance counter.

        if (g_windowData.activeWindows.empty())
            PostQuitMessage(0);

        return 0;
    }
}



namespace Cedar
{
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



    Window Window::Builder::build() const
    {
        registerWindowClass();

        WindowStyles styles = getWindowStyles();

        Size2D<int> size;
        Point2D<int> pos;

        if (m_size.width < 0 || m_size.height < 0)
            size = Size2D<int>(CW_USEDEFAULT, CW_USEDEFAULT);
        else
            size = clientSizeToWindowSize(m_size, styles, USER_DEFAULT_SCREEN_DPI);

        if (m_useDefaultPosition)
            pos = Point2D<int>(CW_USEDEFAULT, CW_USEDEFAULT);
        else
        {
            pos = m_position;

            // Adjusting x and y if they happen to equal CW_USEDEFAULT
            // to avoid the window appearing at the default position.
            if (pos.x == CW_USEDEFAULT)
                pos.x += 1;
            if (pos.y == CW_USEDEFAULT)
                pos.y += 1;
        }

        HWND hWnd = CreateWindowExW(styles.exStyle,
                                    MAKEINTATOM(g_windowData.windowClass),
                                    Platform::Windows::stringToWideString(m_title).c_str(),
                                    styles.style,
                                    pos.x, pos.y,
                                    size.width, size.height,
                                    NULL, NULL,
                                    Platform::Windows::getInstance(),
                                    NULL);

        if (hWnd == NULL)
            throw std::system_error(GetLastError(), std::system_category(),
                                    "Failed to create window");

        // TODO: Add window to instances table then assign the id to window.m_id.
        Window window;
        window.m_handle = NULL;

        ActiveWindow activeWindow;
        activeWindow.window = window;
        activeWindow.styles = styles;
        activeWindow.sizeLimits = m_sizeLimits;
        g_windowData.activeWindows.insert({ hWnd, activeWindow });

        ShowWindow(hWnd, visibilityToCmdShow(m_visiblity));

        return window;
    }



    Window::Window()
    {
        m_handle = NULL;
    }



    bool Window::isOpen() const
    {
        return g_windowData.activeWindows.contains((HWND)m_handle);
    }
}

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

// TODO: Implement Linux version.

#endif // ^^^ Linux ^^^
// OS-specific implementation