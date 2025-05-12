#include "ielement.h"

#include "../color.h"
#include "../math.h"
#include "../terminal.h"

#include <cmath>
#include <memory>
#include <variant>



namespace Cedar::GUI
{
    Color IElement::getBackgroundColor() const
    {
        if (usingParentBackgroundColor() && hasParent())
            return getParent()->getBackgroundColor();
        else
            return m_backgroundColor;
    }



    int IElement::getValueAsInt(std::variant<int, float> value, int parentGlobalSize) const
    {
        if (getValueType(value.index()) == ValueType::absolute)
            return std::get<int>(value);
        else
        {
            float relativeValue = std::get<float>(value);

            if (std::isinf(relativeValue) || std::isnan(relativeValue))
                return 0;
            else
                return static_cast<int>(std::round(relativeValue * parentGlobalSize));
        }
    }



    void IElement::markAsUpdated(UpdateType type)
    {
        m_updated = type;

        if (type != UpdateType::not_updated && type != UpdateType::new_data && hasParent())
            getParent()->markAsUpdated(type);
    }



    Rectangle<int> IElement::globalBounds(const Rectangle<int>& parentGlobalBounds) const
    {
        Rectangle<int> bounds;

        globalBoundsComponent(bounds.topLeft.x, bounds.size.width,
                              m_localBounds.topLeft.x, m_localBounds.size.width,
                              anchorContains(getAnchor(), Anchor::left),
                              anchorContains(getAnchor(), Anchor::right),
                              parentGlobalBounds.topLeft.x, parentGlobalBounds.size.width);

        globalBoundsComponent(bounds.topLeft.y, bounds.size.height,
                              m_localBounds.topLeft.y, m_localBounds.size.height,
                              anchorContains(getAnchor(), Anchor::top),
                              anchorContains(getAnchor(), Anchor::bottom),
                              parentGlobalBounds.topLeft.y, parentGlobalBounds.size.height);

        return bounds;
    }



    void IElement::globalBoundsComponent(int& globalPosition, int& globalSize,
                                         std::variant<int, float> localPosition,
                                         std::variant<int, float> localSize,
                                         bool anchorLeftOrTop, bool anchorRightOrBottom,
                                         int parentGlobalPosition, int parentGlobalSize) const
    {
        globalSize = getValueAsInt(localSize, parentGlobalSize);

        if (anchorLeftOrTop)
            globalPosition = parentGlobalPosition + getValueAsInt(localPosition, parentGlobalSize);
        else if (anchorRightOrBottom)
            globalPosition = parentGlobalPosition + parentGlobalSize - globalSize + getValueAsInt(localPosition, parentGlobalSize);
        else
        {
            globalPosition = parentGlobalPosition + getValueAsInt(localPosition, parentGlobalSize);
            globalPosition += static_cast<int>(std::round((parentGlobalSize - globalSize) / 2.0f));
        }

        globalSize = floor(globalSize, 0);
    }



    void IDrawableElement::render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate)
    {
        if (forceUpdate == false && updated() == UpdateType::not_updated)
            return;

        Rectangle<int> bounds = globalBounds(parentGlobalBounds);

        // URGENT: Change this Array2D's type to be some kind of cell struct with
        //         information about the character and color of each position in the
        //         buffer.
        Array2D<Color> drawBuffer = draw({ static_cast<std::size_t>(bounds.size.width),
                                           static_cast<std::size_t>(bounds.size.height) });

        // IDEA: Restrict foreach to only interate over the part of drawBuffer that's in
        //       view of the terminal rather than checking each individual position.
        drawBuffer.foreach([&](Point2D<std::size_t> bufferPos)
        {
            // TODO: Properly handle narrowing conversion.
            Point2D<int> terminalPos { bufferPos.x + bounds.topLeft.x, bufferPos.y + bounds.topLeft.y };

            if (terminalPos.x >= 0 && terminalPos.x < windowSize.width &&
                terminalPos.y >= 0 && terminalPos.y < windowSize.height)
            {
                // URGENT: Change writing to the terminal to write the character in the
                //         buffer position instead of a space.
                // IDEA: Only set the cursor position at the start of each line.
                // IDEA: Only set the background color when a color change is detected.
                Terminal::setCursorPosition(terminalPos);
                Terminal::setBackgroundColor(drawBuffer.at(bufferPos));
                Terminal::write(' ');
            }
        });

        markAsUpdated(UpdateType::not_updated);
    }



    void ILayoutElement::removeChild(std::size_t index)
    {
        // URGENT: Implement this function.
    }



    void ILayoutElement::removeChildren(std::size_t firstIndex, std::size_t lastIndex)
    {
        // URGENT: Implement this function.
    }
}