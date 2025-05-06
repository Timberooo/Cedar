#include "table.h"

#include "ielement.h"
#include "../math.h"

#include <cstddef>
#include <memory>
#include <variant>
#include <vector>



namespace Cedar::GUI
{
    void Table::resize(Size2D<std::size_t> newSize)
    {
        // URGENT: Properly handle resizing children to account for how the table is
        //         represented. Use Array2D's resize functions as reference.
        resizeChildren(newSize.width * newSize.height);
        m_size = newSize;
        m_rowSize.resize(newSize.height);
        m_columnSize.resize(newSize.width);
        markAsUpdated();
    }



    void Table::render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds)
    {
        //Rectangle<int> bounds = globalBounds(parentGlobalBounds);

        //std::vector<int> rowAbsoluteSizes(m_rowSize.size());
        //int leftoverRowSize = bounds.size.height;

        //for (std::size_t y = 0; y < m_rowSize.size(); y++)
        //{
        //    if (getRowSizeType(y) == ValueType::absolute)
        //    {
        //        rowAbsoluteSizes.at(y) = getAbsoluteRowSize(y);
        //        leftoverRowSize -= getAbsoluteRowSize(y);

        //        if (leftoverRowSize <= 0)
        //            break;
        //    }
        //}



        //for (std::size_t y = 0; y < size().height; y++)
        //    for (std::size_t x = 0; x < size().width; x++)
        //        renderChild(children().at(index(x, y)), windowSize, cellGlobalBounds(bounds));



        Rectangle<int> bounds = globalBounds(parentGlobalBounds);

        for (const auto& child : children())
        {
            renderChild(child, windowSize, bounds);
        }
    }



    Rectangle<int> Table::cellGlobalBounds(const Rectangle<int>& tableGlobalBounds, Point2D<std::size_t> pos) const
    {
        Rectangle<int> cellBounds;

        //cellBounds.size.width = getValueAsInt(m_columnSize[pos.x], tableGlobalBounds.size.width);
        //cellBounds.size.height = getValueAsInt(m_rowSize[pos.y], tableGlobalBounds.size.height);

        //cellBounds.topLeft.x = tableGlobalBounds.topLeft.x +   

        return cellBounds;



        //void IElement::globalBoundsComponent(int& globalPosition, int& globalSize,
        //                                     std::variant<int, float> localPosition,
        //                                     std::variant<int, float> localSize,
        //                                     bool anchorLeftOrTop, bool anchorRightOrBottom,
        //                                     int parentGlobalPosition, int parentGlobalSize) const
        //{
        //    globalSize = getValueAsInt(localSize, parentGlobalSize);

        //    if (anchorLeftOrTop)
        //        globalPosition = parentGlobalPosition + getValueAsInt(localPosition, parentGlobalSize);
        //    else if (anchorRightOrBottom)
        //        globalPosition = parentGlobalPosition + parentGlobalSize - globalSize + getValueAsInt(localPosition, parentGlobalSize);
        //    else
        //    {
        //        globalPosition = parentGlobalPosition + getValueAsInt(localPosition, parentGlobalSize);
        //        globalPosition += static_cast<int>(std::round((parentGlobalSize - globalSize) / 2.0f));
        //    }

        //    globalSize = floor(globalSize, 0);
        //}
    }
}