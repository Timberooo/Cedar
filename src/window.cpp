#include "window.h"

#include "callback.h"
#include "core.h"
#include "io/log.h"

#include <cstddef>
#include <new>
#include <type_traits>

#include <string>



namespace
{
    struct WindowData;
}



// OS-specific definition of WindowData
#if defined(CEDAR_OS_WINDOWS) // vvv Windows vvv

// URGENT: Implement Windows version.

#elif defined(CEDAR_OS_LINUX) // vvv Linux vvv // ^^^ Windows ^^^

#include <X11/Xlib.h>



namespace
{
    struct WindowData
    {
        struct Callbacks
        {
            Cedar::Callback<Cedar::Window::ClosedFunc>     closed;
            Cedar::Callback<Cedar::Window::ClosingFunc>    closing;
            Cedar::Callback<Cedar::Window::ResizedFunc>    resized;
            Cedar::Callback<Cedar::Window::KeyPressedFunc> keyPressed;
        } callbacks;

        struct Protocols
        {
            Atom wm_protocols     = 0;
            Atom wm_delete_window = 0;
        } protocols;


        Display* display = NULL;
        Window   window  = 0;
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

#include <X11/Xlib.h>



namespace
{
    void handleEvent(XEvent* event);
    
    void clientMessageEvent(XEvent* event);

    void exposeEvent(XEvent* event);



    void handleEvent(XEvent* event)
    {
        switch (event->type)
        {
            case ClientMessage:
                clientMessageEvent(event); break;
            case Expose:
                exposeEvent(event); break;
        }
    }



    void clientMessageEvent(XEvent* event)
    {
        if (event->xclient.message_type == g_windowData.protocols.wm_protocols &&
            event->xclient.data.l[0] == g_windowData.protocols.wm_delete_window)
        {
            bool close = true;

            (void)g_windowData.callbacks.closing.tryCall(close);

            if (close)
            {
                (void)g_windowData.callbacks.closed.tryCall();

                XDestroyWindow(g_windowData.display, g_windowData.window);
                XCloseDisplay(g_windowData.display);

                g_windowData.display = NULL;
            }
        }
    }



    void exposeEvent(XEvent* event)
    {
        XDrawString(g_windowData.display, g_windowData.window, DefaultGC(g_windowData.display, 0), 100, 100, "Hello world!", 12);
    }
}



namespace Cedar::Window
{
    void setClosedCallback(ClosedFunc closedCallback)
    {
        g_windowData.callbacks.closed.set(closedCallback);
    }



    void setClosingCallback(ClosingFunc closingCallback)
    {
        g_windowData.callbacks.closing.set(closingCallback);
    }



    void setResizedCallback(ResizedFunc resizedCallback)
    {
        g_windowData.callbacks.resized.set(resizedCallback);
    }



    void setKeyPressedCallback(KeyPressedFunc keyPressedCallback)
    {
        g_windowData.callbacks.keyPressed.set(keyPressedCallback);
    }



    void pollEvents()
    {
        // TODO: XNextEvent blocks while waiting for new events. Need to figure out how
        //       to check for events beforehand to avoid blocking.

        XEvent event;

        XNextEvent(g_windowData.display, &event);
        handleEvent(&event);
    }



    bool isOpen()
    {
        // TODO: Figure out how to detect if the window is still visible. The man page
        //       for XCreateSimpleWindow() doesn't mention a return value that indicates
        //       failure, so using the value of g_windowData.window is probably a bad
        //       idea.
        // return g_windowData.window != NULL;

        return g_windowData.display != NULL;
    }



    void open(std::string_view title, Point2D<int> position, Size2D<int> size, SizeLimits sizeLimits, Mode mode, Visibility visibility)
    {
        g_windowData.display = XOpenDisplay(NULL);

        Point2D<int> pos;
        Size2D<int> winSize;

        pos = position;

        if (size.width < 0 || size.height < 0)
        {
            winSize.width = 100;
            winSize.height = 100;
        }
        else
            winSize = size;

        g_windowData.window = XCreateSimpleWindow(g_windowData.display, DefaultRootWindow(g_windowData.display), pos.x, pos.y, winSize.width, winSize.height, 1, BlackPixel(g_windowData.display, 0), WhitePixel(g_windowData.display, 0));

        XMapWindow(g_windowData.display, g_windowData.window);

        XSelectInput(g_windowData.display, g_windowData.window, ExposureMask);

        g_windowData.protocols.wm_delete_window = XInternAtom(g_windowData.display, "WM_DELETE_WINDOW", False);
        g_windowData.protocols.wm_protocols = XInternAtom(g_windowData.display, "WM_PROTOCOLS", False);
        XSetWMProtocols(g_windowData.display, g_windowData.window, &g_windowData.protocols.wm_delete_window, 1);
    }
}

#endif // ^^^ Linux ^^^
// OS-specific implementation