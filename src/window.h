//
// Window handling features.
//

#ifndef CEDAR_WINDOW_H
#define CEDAR_WINDOW_H

#include "math.h"

#include <cstddef>
#include <limits>
#include <string>
#include <string_view>



namespace Cedar
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



    class Window;

    typedef void (*WindowClosedFunc)(const Window& window);
    typedef bool (*WindowClosingFunc)(Window& window);
    typedef void (*WindowResizedFunc)(Window& window);



    bool windowsOpen();

    void pollEvents();



    class Window
    {
    public:

        enum class Mode {
            windowed
        };

        enum class Visibility {
            show,
            hide,
            maximize,
            minimize
        };


        struct SizeLimits
        {
            Size2D<int> minSize;
            Size2D<int> maxSize;
        };


        class Builder
        {
        public:

            Window build() const;


            inline Builder& title(std::string_view winTitle = "");

            Builder& size(Size2D<int> winSize = { -1, -1 });

            inline Builder& size(int width, int height);

            Builder& maxSize(Size2D<int> winMaxSize = { -1, -1 });

            inline Builder& maxSize(int width, int height);

            Builder& minSize(Size2D<int> winMinSize = { -1, -1 });

            inline Builder& minSize(int width, int height);

            inline Builder& position(Point2D<int> pos = Point2D<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()));

            inline Builder& position(int x, int y);

            inline Builder& mode(Mode winMode = Mode::windowed);

            inline Builder& visibility(Visibility vis = Visibility::show);

        private:

            std::string m_title = "";

            Size2D<int> m_size       = { -1, -1 };
            SizeLimits  m_sizeLimits = { { -1, -1 }, { -1, -1 } };

            bool         m_useDefaultPosition = true;
            Point2D<int> m_position           = { std::numeric_limits<int>::min(), std::numeric_limits<int>::min() };

            Mode       m_mode      = Mode::windowed;
            Visibility m_visiblity = Visibility::show;
        };


        Window();


        bool isOpen() const;

        void close();


        std::string getTitle() const;

        void setTitle(std::string_view title);


        Size2D<int> getSize() const;

        Size2D<int> getMaxSize() const;

        Size2D<int> getMinSize() const;

        void setSize(Size2D<int> size);

        inline void setSize(int width, int height);

        void setMaxSize(Size2D<int> maxSize);

        void setMaxSize(int maxWidth, int maxHeight);

        void setMinSize(Size2D<int> minSize);

        void setMinSize(int minWidth, int minHeight);


        Point2D<int> getPosition() const;

        void setPosition(Point2D<int> pos);

        void setPosition(int x, int y);

    private:

        void* m_handle;
    };



    inline Window::Builder& Window::Builder::title(std::string_view winTitle) {
        m_title = winTitle;
        return *this;
    }



    inline Window::Builder& Window::Builder::size(int width, int height) {
        return size({ width, height });
    }



    inline Window::Builder& Window::Builder::maxSize(int width, int height) {
        return maxSize({ width, height });
    }



    inline Window::Builder& Window::Builder::minSize(int width, int height) {
        return minSize({ width, height });
    }



    inline Window::Builder& Window::Builder::position(Point2D<int> pos)
    {
        m_useDefaultPosition = (pos.x == std::numeric_limits<int>::min() || std::numeric_limits<int>::min());
        m_position = pos;

        return *this;
    }

    inline Window::Builder& Window::Builder::position(int x, int y) {
        return position({ x, y });
    }



    inline Window::Builder& Window::Builder::mode(Mode winMode) {
        m_mode = winMode;
        return *this;
    }



    inline Window::Builder& Window::Builder::visibility(Visibility vis) {
        m_visiblity = vis;
        return *this;
    }
}

#endif // CEDAR_WINDOW_H