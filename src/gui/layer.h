#ifndef CEDAR_GUI_LAYER_H
#define CEDAR_GUI_LAYER_H

#include "ielement.h"
#include "../math.h"



namespace Cedar::GUI
{
    class Layer : public ILayoutElement
    {
    public:

        using ILayoutElement::addChild;

        using ILayoutElement::insertChild;

        using ILayoutElement::setChild;

        using ILayoutElement::removeChild;

        using ILayoutElement::removeChildren;

    protected:

        void render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate) final;
    };
}

#endif // CEDAR_GUI_LAYER_H