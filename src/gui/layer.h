#ifndef CEDAR_GUI_LAYER_H
#define CEDAR_GUI_LAYER_H

#include "ielement.h"
#include "../math.h"

#include <memory>



namespace Cedar::GUI
{
    class Layer final : public ILayoutElement
    {
    public:

        using ILayoutElement::addChild;

        using ILayoutElement::insertChild;

    private:

        void render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds) final;
    };
}

#endif // CEDAR_GUI_LAYER_H