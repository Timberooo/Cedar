#ifndef CEDAR_GUI_WINDOW_H
#define CEDAR_GUI_WINDOW_H

#include "layer.h"
#include "../math.h"
#include "../terminal.h"



namespace Cedar::GUI
{
    class Window final : public Layer
    {
    public:

        void render();

    private:

        Size2D<int> m_lastTerminalSize = Terminal::size();
    };
}

#endif // CEDAR_GUI_WINDOW_H