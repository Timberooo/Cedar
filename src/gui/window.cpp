#include "window.h"



namespace Cedar::GUI
{
    void Window::render()
    {
        Size2D<int> terminalSize = Terminal::size();
        bool update = terminalSize != m_lastTerminalSize || updated() != UpdateType::not_updated;

        if (update)
            Terminal::clear();

        for (const auto& child : children())
            renderChild(child, terminalSize, { 0, 0, terminalSize.width, terminalSize.height }, update);

        m_lastTerminalSize = terminalSize;
        markAsUpdated(UpdateType::not_updated);
    }
}