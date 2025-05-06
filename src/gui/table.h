#ifndef CEDAR_GUI_TABLE_H
#define CEDAR_GUI_TABLE_H

#include "ielement.h"
#include "../math.h"

#include <cstddef>
#include <memory>
#include <variant>
#include <vector>



namespace Cedar::GUI
{
    class Table final : public ILayoutElement
    {
    public:

        template <typename TElement>
        std::shared_ptr<TElement> setChild(Point2D<std::size_t> pos);

        template <typename TElement>
        inline std::shared_ptr<TElement> setChild(std::size_t x, std::size_t y);

        //void removeChild(Point2D<std::size_t> pos);

        //inline void removeChild(std::size_t x, std::size_t y);

        //template <typename TElement>
        //void removeChild(std::shared_ptr<TElement> child);


        inline Size2D<std::size_t> size() const;

        void resize(Size2D<std::size_t> newSize);

        inline void resize(std::size_t width, std::size_t height);


        inline ValueType getRowSizeType(std::size_t row) const;

        inline int getAbsoluteRowSize(std::size_t row) const;

        inline float getRelativeRowSize(std::size_t row) const;

        inline void setAbsoluteRowSize(std::size_t row, int size);

        inline void setRelativeRowSize(std::size_t row, float size);


        inline ValueType getColumnSizeType(std::size_t column) const;

        inline int getAbsoluteColumnSize(std::size_t column) const;

        inline float getRelativeColumnSize(std::size_t column) const;

        inline void setAbsoluteColumnSize(std::size_t column, int size);

        inline void setRelativeColumnSize(std::size_t column, float size);

    private:

        Size2D<std::size_t> m_size;

        std::vector<std::variant<int, float>> m_rowSize;
        std::vector<std::variant<int, float>> m_columnSize;


        void render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds) final;


        inline std::size_t stride() const;

        inline std::size_t index(Point2D<std::size_t> pos) const;

        inline std::size_t index(std::size_t x, std::size_t y) const;


        inline bool inBounds(Point2D<std::size_t> pos) const;

        inline bool inBounds(std::size_t x, std::size_t y) const;


        Rectangle<int> cellGlobalBounds(const Rectangle<int>& tableGlobalBounds, Point2D<std::size_t> pos) const;
    };



    template <typename TElement>
    std::shared_ptr<TElement> Table::setChild(Point2D<std::size_t> pos)
    {
        if (inBounds(pos))
            return ILayoutElement::setChild<TElement>(index(pos));
        else
            throw std::out_of_range("Position was outside the table");
    }

    template <typename TElement>
    inline std::shared_ptr<TElement> Table::setChild(std::size_t x, std::size_t y) {
        return Table::setChild<TElement>({ x, y });
    }



    inline Size2D<std::size_t> Table::size() const {
        return m_size;
    }



    inline void Table::resize(std::size_t width, std::size_t height) {
        resize({ width, height });
    }



    inline Table::ValueType Table::getRowSizeType(std::size_t row) const {
        return getValueType(m_rowSize.at(row).index());
    }



    inline int Table::getAbsoluteRowSize(std::size_t row) const {
        return std::get<int>(m_rowSize.at(row));
    }

    inline float Table::getRelativeRowSize(std::size_t row) const {
        return std::get<float>(m_rowSize.at(row));
    }



    inline void Table::setAbsoluteRowSize(std::size_t row, int size) {
        m_rowSize.at(row) = size;
        markAsUpdated();
    }

    inline void Table::setRelativeRowSize(std::size_t row, float size) {
        m_rowSize.at(row) = size;
        markAsUpdated();
    }



    inline Table::ValueType Table::getColumnSizeType(std::size_t column) const {
        return getValueType(m_columnSize.at(column).index());
    }



    inline int Table::getAbsoluteColumnSize(std::size_t column) const {
        return std::get<int>(m_columnSize.at(column));
    }

    inline float Table::getRelativeColumnSize(std::size_t column) const {
        return std::get<float>(m_columnSize.at(column));
    }



    inline void Table::setAbsoluteColumnSize(std::size_t column, int size) {
        m_columnSize.at(column) = size;
        markAsUpdated();
    }

    inline void Table::setRelativeColumnSize(std::size_t column, float size) {
        m_columnSize.at(column) = size;
        markAsUpdated();
    }



    inline std::size_t Table::stride() const {
        return size().width;
    }



    inline std::size_t Table::index(Point2D<std::size_t> pos) const {
        return pos.x + (pos.y * stride());
    }

    inline std::size_t Table::index(std::size_t x, std::size_t y) const {
        return index({ x, y });
    }



    inline bool Table::inBounds(Point2D<std::size_t> pos) const {
        return (pos.x < size().width && pos.y < size().height);
    }

    inline bool Table::inBounds(std::size_t x, std::size_t y) const {
        return inBounds({ x, y });
    }
}

#endif // CEDAR_GUI_TABLE_H