#ifndef CEDAR_GUI_IELEMENT_H
#define CEDAR_GUI_IELEMENT_H

#include "anchor.h"
#include "../color.h"
#include "../math.h"

#include <cstddef>
#include <memory>
#include <variant>
#include <vector>



namespace Cedar::GUI
{
    class IElement;
    class IDrawableElement;
    class ILayoutElement;



    // vvv IElement vvv

    class IElement : public std::enable_shared_from_this<IElement>
    {
        friend ILayoutElement;

    public:

        enum class ValueType {
            absolute,
            relative
        };


        inline bool hasParent() const;

        inline std::shared_ptr<ILayoutElement> getParent() const;


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

        // NOTE: All update types except not_updated and new_data require the entire tree
        //       of elements to be redrawn, hence the same value being used.
        enum class UpdateType {
            not_updated    = 0,
            new_data       = 1,
            resized        = 2,
            moved          = 2,
            child_modified = 2
        };


        constexpr static Color defaultColor = Color::black;


        virtual void render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate) = 0;


        static inline ValueType getValueType(std::size_t variantIndex);

        int getValueAsInt(std::variant<int, float> value, int parentGlobalSize) const;


        inline UpdateType updated() const;

        void markAsUpdated(UpdateType type);


        Rectangle<int> globalBounds(const Rectangle<int>& parentGlobalBounds) const;

    private:

        UpdateType m_updated = UpdateType::child_modified;

        Anchor                              m_anchor      = Anchor::center;
        Rectangle<std::variant<int, float>> m_localBounds = { 0, 0, 1.0f, 1.0f };

        bool  m_useParentBackgroundColor = true;
        Color m_backgroundColor          = defaultColor;

        std::weak_ptr<ILayoutElement> m_parent;


        void globalBoundsComponent(int& globalPosition, int& globalSize,
                                   std::variant<int, float> localPosition,
                                   std::variant<int, float> localSize,
                                   bool anchorLeftOrTop, bool anchorRightOrBottom,
                                   int parentGlobalPosition, int parentGlobalSize) const;
    };



    inline bool IElement::hasParent() const {
        return !m_parent.expired();
    }



    inline std::shared_ptr<ILayoutElement> IElement::getParent() const {
        return m_parent.lock();
    }



    inline Anchor IElement::getAnchor() const {
        return m_anchor;
    }



    inline void IElement::setAnchor(Anchor anchor) {
        m_anchor = anchor;
        markAsUpdated(UpdateType::moved);
    }



    inline bool IElement::usingParentBackgroundColor() const {
        return m_useParentBackgroundColor;
    }



    inline void IElement::setBackgroundColor() {
        m_useParentBackgroundColor = true;
        m_backgroundColor = defaultColor;
        markAsUpdated(UpdateType::new_data);
    }

    inline void IElement::setBackgroundColor(Color color) {
        m_useParentBackgroundColor = false;
        m_backgroundColor = color;
        markAsUpdated(UpdateType::new_data);
    }



    inline IElement::ValueType IElement::xPositionType() const {
        return getValueType(m_localBounds.topLeft.x.index());
    }



    inline int IElement::getAbsoluteX() const {
        return std::get<int>(m_localBounds.topLeft.x);
    }

    inline float IElement::getRelativeX() const {
        return std::get<float>(m_localBounds.topLeft.x);
    }



    inline void IElement::setAbsoluteX(int x) {
        m_localBounds.topLeft.x = x;
        markAsUpdated(UpdateType::moved);
    }

    inline void IElement::setRelativeX(float x) {
        m_localBounds.topLeft.x = x;
        markAsUpdated(UpdateType::moved);
    }



    inline IElement::ValueType IElement::yPositionType() const {
        return getValueType(m_localBounds.topLeft.y.index());
    }



    inline int IElement::getAbsoluteY() const {
        return std::get<int>(m_localBounds.topLeft.y);
    }

    inline float IElement::getRelativeY() const {
        return std::get<float>(m_localBounds.topLeft.y);
    }



    inline void IElement::setAbsoluteY(int y) {
        m_localBounds.topLeft.y = y;
        markAsUpdated(UpdateType::moved);
    }

    inline void IElement::setRelativeY(float y) {
        m_localBounds.topLeft.y = y;
        markAsUpdated(UpdateType::moved);
    }



    inline IElement::ValueType IElement::widthType() const {
        return getValueType(m_localBounds.size.width.index());
    }



    inline int IElement::getAbsoluteWidth() const {
        return std::get<int>(m_localBounds.size.width);
    }

    inline float IElement::getRelativeWidth() const {
        return std::get<float>(m_localBounds.size.width);
    }



    inline void IElement::setAbsoluteWidth(int width) {
        m_localBounds.size.width = width;
        markAsUpdated(UpdateType::resized);
    }

    inline void IElement::setRelativeWidth(float width) {
        m_localBounds.size.width = width;
        markAsUpdated(UpdateType::resized);
    }



    inline IElement::ValueType IElement::heightType() const {
        return getValueType(m_localBounds.size.height.index());
    }



    inline int IElement::getAbsoluteHeight() const {
        return std::get<int>(m_localBounds.size.height);
    }

    inline float IElement::getRelativeHeight() const {
        return std::get<float>(m_localBounds.size.height);
    }



    inline void IElement::setAbsoluteHeight(int height) {
        m_localBounds.size.height = height;
        markAsUpdated(UpdateType::resized);
    }

    inline void IElement::setRelativeHeight(float height) {
        m_localBounds.size.height = height;
        markAsUpdated(UpdateType::resized);
    }



    inline IElement::ValueType IElement::getValueType(std::size_t variantIndex) {
        return (variantIndex == 0 ? ValueType::absolute : ValueType::relative);
    }



    inline IElement::UpdateType IElement::updated() const {
        return m_updated;
    }



    // vvv IDrawableElement vvv // ^^^ IElement ^^^

    class IDrawableElement : public IElement
    {
    protected:

        void render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate) final;


        virtual Array2D<Color> draw(Size2D<std::size_t> bufferSize) const = 0;
    };



    // vvv ILayoutElement vvv // ^^^ IDrawableElement ^^^

    class ILayoutElement : public IElement
    {
    public:

        inline bool hasChildren() const;

    protected:

        template <typename TElement>
        std::shared_ptr<TElement> addChild();

        template <typename TElement>
        std::shared_ptr<TElement> insertChild(std::size_t index);

        template <typename TElement>
        std::shared_ptr<TElement> setChild(std::size_t index);


        template <typename TElement>
        void removeChild(std::shared_ptr<TElement> child);

        void removeChild(std::size_t index);

        void removeChildren(std::size_t firstIndex, std::size_t lastIndex);


        inline const std::vector<std::shared_ptr<IElement>>& children() const;

        inline void resizeChildren(std::size_t newSize);


        inline void renderChild(std::shared_ptr<IElement> child, Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate);

    private:

        std::vector<std::shared_ptr<IElement>> m_children;

        // TODO: Implement viewport support, including automatic scroll bar rendering
        //       when the viewport is smaller than m_localBounds.
        // TODO: Implement viewport culling by skipping render calls to objects that are
        //       completely outside of the viewport.
        Rectangle<std::variant<int, float>> m_viewport = { 0, 0, 1.0f, 1.0f };
    };



    inline bool ILayoutElement::hasChildren() const {
        return !m_children.empty();
    }



    template <typename TElement>
    std::shared_ptr<TElement> ILayoutElement::addChild()
    {
        std::shared_ptr<TElement> child = std::make_shared<TElement>();

        m_children.push_back(std::static_pointer_cast<IElement>(child));
        child->m_parent = std::static_pointer_cast<ILayoutElement>(shared_from_this());
        markAsUpdated(UpdateType::child_modified);

        return child;
    }



    template <typename TElement>
    std::shared_ptr<TElement> ILayoutElement::insertChild(std::size_t index)
    {
        std::shared_ptr<TElement> child = std::make_shared<TElement>();

        m_children.insert(m_children.begin() + index, std::static_pointer_cast<IElement>(child));
        child->m_parent = std::static_pointer_cast<ILayoutElement>(shared_from_this());
        markAsUpdated(UpdateType::child_modified);

        return child;
    }



    template <typename TElement>
    std::shared_ptr<TElement> ILayoutElement::setChild(std::size_t index)
    {
        std::shared_ptr<TElement> child = std::make_shared<TElement>();

        m_children.at(index) = std::static_pointer_cast<IElement>(child);
        child->m_parent = std::static_pointer_cast<ILayoutElement>(shared_from_this());
        markAsUpdated(UpdateType::child_modified);

        return child;
    }



    template <typename TElement>
    void ILayoutElement::removeChild(std::shared_ptr<TElement> child)
    {
        // URGENT: Implement this function.
    }



    inline const std::vector<std::shared_ptr<IElement>>& ILayoutElement::children() const {
        return m_children;
    }



    inline void ILayoutElement::resizeChildren(std::size_t newSize) {
        m_children.resize(newSize);
        markAsUpdated(UpdateType::child_modified);
    }



    inline void ILayoutElement::renderChild(std::shared_ptr<IElement> child, Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate) {
        // TODO: Figure out why this function is necessary because I forgor.
        child->render(windowSize, parentGlobalBounds, forceUpdate);
    }
}

#endif // CEDAR_GUI_IELEMENT_H