#include "window.h"



namespace Cedar::GUI
{
    void Window::render()
    {
        Size2D<int> terminalSize = Terminal::size();
        bool resized = terminalSize != m_lastTerminalSize;

        for (const auto& child : children())
            renderChild(child, terminalSize, { 0, 0, terminalSize.width, terminalSize.height }, resized);

        m_lastTerminalSize = terminalSize;
        markAsUpdated(UpdateType::not_updated);
    }
}