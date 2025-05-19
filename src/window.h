//#ifndef CEDAR_WINDOW_H
//#define CEDAR_WINDOW_H
//
//#include "math.h"
//
//#include "niftyCounter.h"
//
//#include <cstddef>
//#include <string>
//
//
//
//namespace Cedar
//{
//    CEDAR_NIFTY_COUNTER_HEADER(WindowInitializer);
//
//
//
//    class Window;
//    class WindowBuilder;
//
//    typedef void (*WindowClosedFunc)(const Window& window);
//    typedef bool (*WindowClosingFunc)(Window& window);
//    typedef void (*WindowResizedFunc)(Window& window);
//
//
//
//    bool windowsOpen();
//
//    void pollEvents();
//
//
//
//    class Window
//    {
//    public:
//
//        enum class Mode {
//            windowed
//        };
//
//        enum class Visibility {
//            show,
//            hide,
//            maximize,
//            minimize
//        };
//
//
//        class Builder
//        {
//        public:
//
//            Window build() const;
//
//
//            inline Builder& title(std::string_view winTitle);
//
//            inline Builder& size(Size2D<int> winSize);
//
//            inline Builder& size(int width, int height);
//
//            inline Builder& maxSize(Size2D<int> winMaxSize);
//
//            inline Builder& maxSize(int width, int height);
//
//            inline Builder& minSize(Size2D<int> winMinSize);
//
//            inline Builder& minSize(int width, int height);
//
//            inline Builder& position(Point2D<int> pos);
//
//            inline Builder& position(int x, int y);
//
//            inline Builder& mode(Mode winMode);
//
//            inline Builder& visibility(Visibility vis);
//
//        private:
//
//            std::string m_title = "";
//
//            Size2D<int> m_size    = { -1, -1 };
//            Size2D<int> m_maxSize = { -1, -1 };
//            Size2D<int> m_minSize = { -1, -1 };
//
//            Point2D<int> m_position; // TODO
//
//            Mode       m_mode       = Mode::windowed;
//            Visibility m_visibility = Visibility::show;
//        };
//
//
//        bool isOpen() const;
//
//        void close();
//
//
//        Size2D<int> getSize() const;
//
//        Size2D<int> getMaxSize() const;
//
//        Size2D<int> getMinSize() const;
//
//        void setSize(Size2D<int> size);
//
//        inline void setSize(int width, int height);
//
//        void setMaxSize(Size2D<int> maxSize);
//
//        inline void setMaxSize(int maxWidth, int maxHeight);
//
//        void setMinSize(Size2D<int> minSize);
//
//        inline void setMinSize(int minWidth, int minHeight);
//
//
//        std::string getTitle() const;
//
//        void setTitle(std::string_view title);
//
//    private:
//
//
//    };
//
//
//
//    inline Window::Builder& Window::Builder::title(std::string_view winTitle) {
//        m_title = winTitle;
//        return *this;
//    }
//
//    inline Window::Builder& Window::Builder::size(Size2D<int> winSize) {
//        m_size = winSize;
//        return *this;
//    }
//
//    inline Window::Builder& Window::Builder::size(int width, int height) {
//        return size({ width, height });
//    }
//
//    inline Window::Builder& Window::Builder::maxSize(Size2D<int> winMaxSize) {
//        m_maxSize = winMaxSize;
//        return *this;
//    }
//
//    inline Window::Builder& Window::Builder::maxSize(int width, int height) {
//        return maxSize({ width, height });
//    }
//
//    inline Window::Builder& Window::Builder::minSize(Size2D<int> winMinSize) {
//        m_minSize = winMinSize;
//        return *this;
//    }
//
//    inline Window::Builder& Window::Builder::minSize(int width, int height) {
//        return minSize({ width, height });
//    }
//
//    inline Window::Builder& Window::Builder::position(Point2D<int> pos) {
//        m_position = pos;
//        return *this;
//    }
//
//    inline Window::Builder& Window::Builder::position(int x, int y) {
//        return position({ x, y });
//    }
//
//    inline Window::Builder& Window::Builder::mode(Mode winMode) {
//        m_mode = winMode;
//        return *this;
//    }
//
//    inline Window::Builder& Window::Builder::visibility(Visibility vis) {
//        m_visibility = vis;
//        return *this;
//    }
//}
//
//#endif // CEDAR_WINDOW_H