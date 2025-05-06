#include "layer.h"

#include "ielement.h"
#include "../math.h"

#include <memory>
#include <vector>



namespace Cedar::GUI
{
    void Layer::render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds)
    {
        Rectangle<int> bounds = globalBounds(parentGlobalBounds);

        for (const auto& child : children())
            renderChild(child, windowSize, bounds);

        markAsUpdated(false);
    }
}