//
// Window handling features.
//

#ifndef CEDAR_WINDOW_H
#define CEDAR_WINDOW_H

#include "input.h"
#include "math.h"

#include <cstddef>
#include <string>
#include <string_view>



namespace Cedar::Window
{
    // Nifty counter. For internal use only
    class WindowInitializer
    {
    public:

        WindowInitializer();

        ~WindowInitializer();

    private:

        static std::size_t s_counter;
    };



    // Nifty counter. For internal use only
    static WindowInitializer windowInitializer;


    
    enum class Mode {
        Windowed,
        Fullscreen,
        Fullscreen_Borderless
    };

    enum class Visibility {
        Show,
        Hide,
        Minimize,
        Maximize
    };



    typedef void (*ClosedFunc)();
    typedef bool (*ClosingFunc)();
    typedef void (*ResizedFunc)();
    typedef void (*KeyPressedFunc)(Key key);
    typedef void (*VisibilityChangedFunc)();



    struct SizeLimits
    {
        Size2D<int> minSize;
        Size2D<int> maxSize;
    };


    
    void setClosedCallback(ClosedFunc closedCallback);

    void setClosingCallback(ClosingFunc closingCallback);

    void setResizedCallback(ResizedFunc resizedCallback);

    void setKeyPressedCallback(KeyPressedFunc keyPressedCallback);


    void pollEvents();


    bool isOpen();

    void open(std::string_view title = "", Point2D<int> position = { -1, -1 }, Size2D<int> size = { -1, -1 }, SizeLimits sizeLimits = { { -1, -1 }, { -1, -1 } }, Mode mode = Mode::Windowed, Visibility visibility = Visibility::Show);

    void close();


    std::string getTitle();

    void setTitle(std::string_view title);


    Point2D<int> getPosition();

    void setPosition(Point2D<int> position);


    Size2D<int> getSize();

    void setSize(Size2D<int> size);

    void setSize(int width, int height);


    SizeLimits getSizeLimits();

    void setSizeLimits(SizeLimits sizeLimits);

    void setSizeLimits(Size2D<int> minSize, Size2D<int> maxSize);

    void setSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight);

    
    Size2D<int> getMinSize();

    void setMinSize(Size2D<int> minSize);

    void setMinSize(int minWidth, int minHeight);


    Size2D<int> getMaxSize();

    void setMaxSize(Size2D<int> maxSize);

    void setMaxSize(int maxWidth, int maxHeight);


    Mode getMode();

    void setMode(Mode mode);


    Visibility getVisibility();

    void setVisibility(Visibility visibility);
}

#endif // CEDAR_WINDOW_H