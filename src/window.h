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
        Windowed,
        Fullscreen,
        Fullscreen_borderless
    };

    enum class Visibility {
        Show,
        Hide,
        Minimize,
        Maximize
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
        static constexpr Mode         defaultMode       = Mode::Windowed;
        static constexpr Visibility   defaultVisibility = Visibility::Show;


        inline OpenArgs& title(std::string_view windowTitle = defaultTitle);

        OpenArgs& position(Point2D<int> windowPosition = defaultPosition);

        inline OpenArgs& position(int x, int y);

        OpenArgs& size(Size2D<int> windowSize = defaultSize);

        inline OpenArgs& size(int width, int height);

        OpenArgs& sizeLimits(SizeLimits windowSizeLimits = { defaultSize, defaultSize });

        inline OpenArgs& sizeLimits(Size2D<int> minSize, Size2D<int> maxSize);

        inline OpenArgs& sizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight);

        OpenArgs& minSize(Size2D<int> windowMinSize = defaultSize);

        inline OpenArgs& minSize(int minWidth, int minHeight);

        OpenArgs& maxSize(Size2D<int> windowMaxSize = defaultSize);

        inline OpenArgs& maxSize(int maxWidth, int maxHeight);

        inline OpenArgs& mode(Mode windowMode = defaultMode);

        inline OpenArgs& visibility(Visibility windowVisibility = defaultVisibility);


        inline std::string getTitle() const;

        inline Point2D<int> getPosition() const;

        inline Size2D<int> getSize() const;

        inline SizeLimits getSizeLimits() const;

        inline Mode getMode() const;

        inline Visibility getVisibility() const;

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

    void setSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight);

    void setMinSize(Size2D<int> minSize);

    void setMinSize(int minWidth, int minHeight);

    void setMaxSize(Size2D<int> maxSize);

    void setMaxSize(int maxWidth, int maxHeight);

    void setMode(Mode mode);

    void setVisibility(Visibility visibility);



    // vvv OpenArgs function definitions vvv

    inline OpenArgs& OpenArgs::title(std::string_view windowTitle) {
        m_title = windowTitle;
        return *this;
    }



    inline OpenArgs& OpenArgs::position(int x, int y) {
        return position({ x, y });
    }



    inline OpenArgs& OpenArgs::size(int width, int height) {
        return size({ width, height });
    }



    inline OpenArgs& OpenArgs::sizeLimits(Size2D<int> minSize, Size2D<int> maxSize) {
        return sizeLimits({ minSize, maxSize });
    }

    inline OpenArgs& OpenArgs::sizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) {
        return sizeLimits({ minWidth, minHeight, maxWidth, maxHeight });
    }



    inline OpenArgs& OpenArgs::minSize(int minWidth, int minHeight) {
        return minSize({ minWidth, minHeight });
    }



    inline OpenArgs& OpenArgs::maxSize(int maxWidth, int maxHeight) {
        return maxSize({ maxWidth, maxHeight });
    }



    inline OpenArgs& OpenArgs::mode(Mode windowMode) {
        m_mode = windowMode;
        return *this;
    }



    inline OpenArgs& OpenArgs::visibility(Visibility windowVisibility) {
        m_visibility = windowVisibility;
        return *this;
    }



    inline std::string OpenArgs::getTitle() const {
        return m_title;
    }



    inline Point2D<int> OpenArgs::getPosition() const {
        return m_position;
    }



    inline Size2D<int> OpenArgs::getSize() const {
        return m_size;
    }



    inline SizeLimits OpenArgs::getSizeLimits() const {
        return m_sizeLimits;
    }



    inline Mode OpenArgs::getMode() const {
        return m_mode;
    }



    inline Visibility OpenArgs::getVisibility() const {
        return m_visibility;
    }

    // ^^^ OpenArgs function definitions ^^^
}

#endif // CEDAR_WINDOW_H