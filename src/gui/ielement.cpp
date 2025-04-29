#include "ielement.h"

#include "../color.h"
#include "../math.h"
#include "../terminal.h"

#include <cmath>
#include <cstddef>
#include <memory>
#include <variant>
#include <vector>



namespace Cedar::GUI
{
    Color IElement::getBackgroundColor() const
    {
        if (usingParentBackgroundColor() && hasParent())
            return getParent()->getBackgroundColor();
        else
            return m_backgroundColor;
    }



    void IElement::markAsUpdated()
    {
        // TODO
    }



    Rectangle<int> IElement::calculateBounds(const Rectangle<int>& limitBounds) const
    {
        Rectangle<int> bounds;

        calculateDimensionBounds(bounds.topLeft.x, bounds.size.width,
                                 m_bounds.topLeft.x, m_bounds.size.width,
                                 anchorContains(getAnchor(), Anchor::left),
                                 anchorContains(getAnchor(), Anchor::right),
                                 limitBounds.topLeft.x, limitBounds.size.width);

        calculateDimensionBounds(bounds.topLeft.y, bounds.size.height,
                                 m_bounds.topLeft.y, m_bounds.size.height,
                                 anchorContains(getAnchor(), Anchor::top),
                                 anchorContains(getAnchor(), Anchor::bottom),
                                 limitBounds.topLeft.y, limitBounds.size.height);

        return bounds;
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
        else
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



    void IDrawableElement::render(Size2D<int> windowSize, const Rectangle<int> limitBounds)
    {
        if (!updated())
            return;

        Rectangle<int> bounds = calculateBounds(limitBounds);

        Size2D<std::size_t> size;
        size.width = bounds.size.width;
        size.height = bounds.size.height;

        Array2D<Color> output = draw(size);

        for (std::size_t y = 0; y < output.size().height; y++)
        {
            for (std::size_t x = 0; x < output.size().width; x++)
            {
                Point2D<int> pos;
                pos.x = x + bounds.topLeft.x;
                pos.y = y + bounds.topLeft.y;

                if (pos.x >= 0 && pos.x <= windowSize.width &&
                    pos.y >= 0 && pos.y <= windowSize.height)
                {
                    Terminal::setCursorPosition(pos.x, pos.y);
                    Terminal::setBackgroundColor(output.at(x, y));
                    Terminal::write(' ');
                }
            }
        }

        m_updated = false;
    }



    void LayoutLayer::render(Size2D<int> windowSize, const Rectangle<int> limitBounds)
    {
        Rectangle<int> bounds = calculateBounds(limitBounds);

        for (const auto child : m_children)
        {
            child->render(windowSize, bounds);
        }

        m_updated = false;
    }
}