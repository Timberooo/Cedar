//
// Window handling features.
//

#ifndef CEDAR_WINDOW_H
#define CEDAR_WINDOW_H

#include "input.h"
#include "math.h"

#include <cstddef>
#include <limits>
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
        windowed,
        fullscreen,
        fullscreen_borderless
    };

    enum class Visibility {
        show,
        hide,
        minimize,
        maximize
    };



    typedef void (*ClosedFunc)();
    typedef bool (*ClosingFunc)();
    typedef void (*KeyPressedFunc)(Key key);
    typedef void (*ResizedFunc)();
    typedef void (*VisibilityChangedFunc)();


    
    struct SizeLimits;

    class OpenArgs;



    struct SizeLimits
    {
        Size2D<int> minSize;
        Size2D<int> maxSize;
    };



    class OpenArgs
    {
    public:

        static constexpr const char*  defaultTitle      = "";
        static constexpr Point2D<int> defaultPosition   = { std::numeric_limits<int>::min(), std::numeric_limits<int>::min() };
        static constexpr Size2D<int>  defaultSize       = { -1, -1 };
        static constexpr Mode         defaultMode       = Mode::windowed;
        static constexpr Visibility   defaultVisibility = Visibility::show;


        OpenArgs& title(std::string_view title = defaultTitle);

        OpenArgs& position(Point2D<int> position = defaultPosition);

        OpenArgs& position(int x, int y);

        OpenArgs& size(Size2D<int> size = defaultSize);

        OpenArgs& size(int width, int height);

        OpenArgs& sizeLimits(SizeLimits sizeLimits = { defaultSize, defaultSize });

        OpenArgs& sizeLimits(Size2D<int> minSize, Size2D<int> maxSize);

        OpenArgs& sizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight);

        OpenArgs& minSize(Size2D<int> minSize = defaultSize);

        OpenArgs& minSize(int minWidth, int minHeight);

        OpenArgs& maxSize(Size2D<int> maxSize = defaultSize);

        OpenArgs& maxSize(int maxWidth, int maxHeight);

        OpenArgs& mode(Mode mode = defaultMode);

        OpenArgs& visibility(Visibility visibility = defaultVisibility);


        std::string getTitle() const;

        Point2D<int> getPosition() const;

        Size2D<int> getSize() const;

        SizeLimits getSizeLimits() const;

        Mode getMode() const;

        Visibility getVisibility() const;

    private:

        std::string  m_title      = defaultTitle;
        Point2D<int> m_position   = defaultPosition;
        Size2D<int>  m_size       = defaultSize;
        SizeLimits   m_sizeLimits = { defaultSize, defaultSize };
        Mode         m_mode       = defaultMode;
        Visibility   m_visibility = defaultVisibility;
    };



    bool isOpen();

    void open(const OpenArgs& openArgs = OpenArgs());

    void close();


    void pollEvents();


    ClosedFunc getClosedCallback();

    ClosingFunc getClosingCallback();

    KeyPressedFunc getKeyPressedCallback();

    ResizedFunc getResizedCallback();

    VisibilityChangedFunc getVisibilityChangedCallback();


    void setClosedCallback(ClosedFunc closedCallback);

    void setClosingCallback(ClosingFunc closingCallback);

    void setKeyPressedCallback(KeyPressedFunc keyPressedCallback);

    void setResizedCallback(ResizedFunc resizedCallback);

    void setVisibilityChangedCallback(VisibilityChangedFunc visibilityChangedCallback);


    std::string getTitle();

    Point2D<int> getPosition();

    Size2D<int> getSize();

    SizeLimits getSizeLimits();

    Mode getMode();

    Visibility getVisibility();


    void setTitle(std::string_view title);

    void setPosition(Point2D<int> position);

    void setPosition(int x, int y);

    void setSize(Size2D<int> size);

    void setSize(int width, int height);

    void setSizeLimits(SizeLimits sizeLimits);

    void setSizeLimits(Size2D<int> minSize, Size2D<int> maxSize);

    void setSizeLimits(int minwidth, int minHeight, int maxWidth, int maxHeight);

    void setMinSize(Size2D<int> minSize);

    void setMinSize(int minWidth, int minHeight);

    void setMaxSize(Size2D<int> maxSize);

    void setMaxSize(int maxWidth, int maxHeight);

    void setMode(Mode mode);

    void setVisibility(Visibility visibility);
}

#endif // CEDAR_WINDOW_H