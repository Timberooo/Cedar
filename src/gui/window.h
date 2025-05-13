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


        // NOTE: std::enable_shared_from_this requires all derived classes to be
        //       inherited publicly, so irrelevant functions from IElement also become
        //       public. The best solution to this seems to be either deleting irrelevant
        //       functions or making them private through using declarations. Neither
        //       approach stops calling code from using fully qualified name lookups to
        //       call functions (ex: window->IElement::hasParent()) or prevents them from
        //       appearing in suggestions in an IDE, but deleting them at least conveys
        //       the intent for these functions to not be used.

        bool hasParent() const = delete;
        std::shared_ptr<ILayoutElement> getParent() const = delete;
        Anchor getAnchor() const = delete;
        void setAnchor(Anchor) = delete;
        bool usingParentBackgroundColor() const = delete;
        ValueType xPositionType() const = delete;
        int getAbsoluteX() const = delete;
        float getRelativeX() const = delete;
        void setAbsoluteX(int) = delete;
        void setRelativeX(float) = delete;
        ValueType yPositionType() const = delete;
        int getAbsoluteY() const = delete;
        float getRelativeY() const = delete;
        void setAbsoluteY(int) = delete;
        void setRelativeY(float) = delete;
        ValueType widthType() const = delete;
        int getAbsoluteWidth() const = delete;
        float getRelativeWidth() const = delete;
        void setAbsoluteWidth(int) = delete;
        void setRelativeWidth(float) = delete;
        ValueType heightType() const = delete;
        int getAbsoluteHeight() const = delete;
        float getRelativeHeight() const = delete;
        void setAbsoluteHeight(int) = delete;
        void setRelativeHeight(float) = delete;

    private:

        Size2D<int> m_lastTerminalSize = Terminal::size();
    };
}

#endif // CEDAR_GUI_WINDOW_H