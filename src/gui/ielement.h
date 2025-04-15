#ifndef CEDAR_GUI_IELEMENT_H
#define CEDAR_GUI_IELEMENT_H

#include "anchor.h"
#include "../color.h"
#include "../math.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>



namespace Cedar::GUI
{
    typedef char CP437Char;

    class IElement;



    class IElement : public std::enable_shared_from_this<IElement>
    {
    public:

        enum class ValueType {
            absolute,
            relative
        };

        struct Cell {
            Color     color;
            CP437Char character;
        };


        void draw(Size2D<int> windowSize, const Rectangle<int>& parentBounds);


        inline bool isParent() const;

        inline bool isChild() const;

        inline std::shared_ptr<IElement> getParent() const;

        template <typename TElement>
        std::shared_ptr<TElement> addChild();

        template <typename TElement>
        void removeChild(std::shared_ptr<TElement> child);


        inline Anchor getAnchor() const;

        inline void setAnchor(Anchor anchor);


        inline bool usingParentBackgroundColor() const;

        Color getBackgroundColor() const;

        inline void setBackgroundColor();

        inline void setBackgroundColor(Color color);


        inline ValueType xPositionType() const;

        inline int getAbsoluteX() const;

        inline float getRelativeX() const;

        inline void setAbsoluteX(int x);

        inline void setRelativeX(float x);


        inline ValueType yPositionType() const;

        inline int getAbsoluteY() const;

        inline float getRelativeY() const;

        inline void setAbsoluteY(int y);

        inline void setRelativeY(float y);


        inline ValueType widthType() const;

        inline int getAbsoluteWidth() const;

        inline float getRelativeWidth() const;

        inline void setAbsoluteWidth(int width);

        inline void setRelativeWidth(float width);


        inline ValueType heightType() const;

        inline int getAbsoluteHeight() const;

        inline float getRelativeHeight() const;

        inline void setAbsoluteHeight(int height);

        inline void setRelativeHeight(float height);

    protected:

        constexpr static Color defaultColor = Color::black;


        virtual Array2D<Cell> render(Size2D<std::size_t> size) const = 0;


        void markAsUpdated();

    private:

        struct DimensionBounds {
            int position;
            int size;
        };


        bool m_updated = true;

        Anchor                              m_anchor = Anchor::center;
        Rectangle<std::variant<int, float>> m_bounds = { { 0, 0 }, { 0, 0 } };

        bool  m_useParentBackgroundColor = true;
        Color m_backgroundColor          = defaultColor;

        std::weak_ptr<IElement>                m_parent;
        std::vector<std::shared_ptr<IElement>> m_children;


        void printToTerminal(const Array2D<Cell>& renderOutput, Rectangle<int> bounds, Size2D<int> windowSize);

        static inline ValueType getValueType(std::size_t variantIndex);


        Rectangle<int> calculateBounds(const Rectangle<int>& parentBounds) const;

        void calculateDimensionBounds(int& boundsPosition, int& boundsSize,
                                      std::variant<int, float> position,
                                      std::variant<int, float> size,
                                      bool anchorLeftOrTop, bool anchorRightOrBottom,
                                      int parentPosition, int parentSize) const;

        int getValueAsInt(std::variant<int, float> value, int parentDimensionSize) const;
    };



    inline bool IElement::isParent() const {
        return !m_children.empty();
    }



    inline bool IElement::isChild() const {
        return !m_parent.expired();
    }



    inline std::shared_ptr<IElement> IElement::getParent() const {
        return m_parent.lock();
    }



    template <typename TElement>
    std::shared_ptr<TElement> IElement::addChild()
    {
        std::shared_ptr<TElement> child = std::make_shared<TElement>();

        m_children.push_back(std::static_pointer_cast<IElement>(child));
        child->m_parent = shared_from_this();
        markAsUpdated();

        return child;
    }



    template <typename TElement>
    void IElement::removeChild(std::shared_ptr<TElement> child)
    {
        for (auto it = m_children.begin(); it != m_children.end(); it++)
        {
            if (*it == std::static_pointer_cast<IElement>(child))
            {
                m_children.erase(it);
                markAsUpdated();
                return;
            }
        }

        throw std::invalid_argument("Invalid child element");
    }



    inline Anchor IElement::getAnchor() const {
        return m_anchor;
    }



    inline void IElement::setAnchor(Anchor anchor) {
        m_anchor = anchor;
        markAsUpdated();
    }



    inline bool IElement::usingParentBackgroundColor() const {
        return m_useParentBackgroundColor;
    }



    inline void IElement::setBackgroundColor() {
        m_useParentBackgroundColor = true;
        m_backgroundColor = defaultColor;
        markAsUpdated();
    }



    inline void IElement::setBackgroundColor(Color color) {
        m_useParentBackgroundColor = false;
        m_backgroundColor = color;
        markAsUpdated();
    }



    inline IElement::ValueType IElement::xPositionType() const {
        return getValueType(m_bounds.topLeft.x.index());
    }



    inline int IElement::getAbsoluteX() const {
        return std::get<int>(m_bounds.topLeft.x);
    }

    inline float IElement::getRelativeX() const {
        return std::get<float>(m_bounds.topLeft.x);
    }



    inline void IElement::setAbsoluteX(int x) {
        m_bounds.topLeft.x = x;
        markAsUpdated();
    }

    inline void IElement::setRelativeX(float x) {
        m_bounds.topLeft.x = x;
        markAsUpdated();
    }



    inline IElement::ValueType IElement::yPositionType() const {
        return getValueType(m_bounds.topLeft.y.index());
    }



    inline int IElement::getAbsoluteY() const {
        return std::get<int>(m_bounds.topLeft.y);
    }

    inline float IElement::getRelativeY() const {
        return std::get<float>(m_bounds.topLeft.y);
    }



    inline void IElement::setAbsoluteY(int y) {
        m_bounds.topLeft.y = y;
        markAsUpdated();
    }

    inline void IElement::setRelativeY(float y) {
        m_bounds.topLeft.y = y;
        markAsUpdated();
    }



    inline IElement::ValueType IElement::widthType() const {
        return getValueType(m_bounds.size.width.index());
    }



    inline int IElement::getAbsoluteWidth() const {
        return std::get<int>(m_bounds.size.width);
    }

    inline float IElement::getRelativeWidth() const {
        return std::get<float>(m_bounds.size.width);
    }



    inline void IElement::setAbsoluteWidth(int width) {
        m_bounds.size.width = width;
        markAsUpdated();
    }

    inline void IElement::setRelativeWidth(float width) {
        m_bounds.size.width = width;
        markAsUpdated();
    }



    inline IElement::ValueType IElement::heightType() const {
        return getValueType(m_bounds.size.height.index());
    }



    inline int IElement::getAbsoluteHeight() const {
        return std::get<int>(m_bounds.size.height);
    }

    inline float IElement::getRelativeHeight() const {
        return std::get<float>(m_bounds.size.height);
    }



    inline void IElement::setAbsoluteHeight(int height) {
        m_bounds.size.height = height;
        markAsUpdated();
    }

    inline void IElement::setRelativeHeight(float height) {
        m_bounds.size.height = height;
        markAsUpdated();
    }



    inline IElement::ValueType IElement::getValueType(std::size_t variantIndex) {
        return (variantIndex == 0 ? ValueType::absolute : ValueType::relative);
    }
}

#endif // CEDAR_GUI_IELEMENT_H