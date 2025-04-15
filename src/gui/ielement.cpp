#include "ielement.h"

#include "../terminal.h"

#include <cmath>



namespace Cedar::GUI
{
    void IElement::draw(Size2D<int> windowSize, const Rectangle<int>& parentBounds)
    {
        Rectangle<int> bounds = calculateBounds(parentBounds);

        // TODO: Handle the conversion from int to size_t properly.
        Size2D<std::size_t> size;
        size.width = bounds.size.width;
        size.height = bounds.size.height;

        if (m_updated)
            printToTerminal(render(size), bounds, windowSize);

        for (const auto child : m_children)
            child->draw(windowSize, bounds);

        m_updated = false;
    }



    Color IElement::getBackgroundColor() const
    {
        if (isChild() && usingParentBackgroundColor())
            return getParent()->getBackgroundColor();
        else
            return m_backgroundColor;
    }



    Rectangle<int> IElement::calculateBounds(const Rectangle<int>& parentBounds) const
    {
        Rectangle<int> bounds;

        calculateDimensionBounds(bounds.topLeft.x, bounds.size.width,
                                 m_bounds.topLeft.x, m_bounds.size.width,
                                 anchorContains(getAnchor(), Anchor::left),
                                 anchorContains(getAnchor(), Anchor::right),
                                 parentBounds.topLeft.x, parentBounds.size.width);

        calculateDimensionBounds(bounds.topLeft.y, bounds.size.height,
                                 m_bounds.topLeft.y, m_bounds.size.height,
                                 anchorContains(getAnchor(), Anchor::top),
                                 anchorContains(getAnchor(), Anchor::bottom),
                                 parentBounds.topLeft.y, parentBounds.size.height);

        return bounds;
    }



    void IElement::markAsUpdated()
    {
        m_updated = true;
    }



    void IElement::printToTerminal(const Array2D<Cell>& renderOutput, Rectangle<int> bounds, Size2D<int> windowSize)
    {
        for (std::size_t y = 0; y < renderOutput.size().height; y++)
        {
            for (std::size_t x = 0; x < renderOutput.size().width; x++)
            {
                Point2D<int> pos;
                pos.x = x + bounds.topLeft.x;
                pos.y = y + bounds.topLeft.y;

                if (pos.x >= 0 && pos.x <= windowSize.width &&
                    pos.y >= 0 && pos.y <= windowSize.height)
                {
                    Terminal::setCursorPosition(pos.x, pos.y);
                    Terminal::setBackgroundColor(renderOutput.at(x, y).color);
                    Terminal::write(renderOutput.at(x, y).character);
                }
            }
        }
    }



    void IElement::calculateDimensionBounds(int& boundsPosition, int& boundsSize,
                                            std::variant<int, float> position,
                                            std::variant<int, float> size,
                                            bool anchorLeftOrTop, bool anchorRightOrBottom,
                                            int parentPosition, int parentSize) const
    {
        boundsSize = getValueAsInt(size, parentSize);

        if (anchorLeftOrTop)
            boundsPosition = parentPosition + getValueAsInt(position, parentSize);
        else if (anchorRightOrBottom)
            boundsPosition = parentPosition + parentSize - boundsSize + getValueAsInt(position, parentSize);
        else // Anchored at center in this dimension
        {
            boundsPosition = parentPosition + getValueAsInt(position, parentSize);
            boundsPosition += static_cast<int>(std::round((parentSize - boundsSize) / 2.0f));
        }
    }



    int IElement::getValueAsInt(std::variant<int, float> value, int parentDimensionSize) const
    {
        if (getValueType(value.index()) == ValueType::absolute)
            return std::get<int>(value);
        else
        {
            float relativeVal = std::get<float>(value);

            if (std::isinf(relativeVal) || std::isnan(relativeVal))
                return 0;
            else
                return static_cast<int>(std::round(relativeVal * parentDimensionSize));
        }
    }
}