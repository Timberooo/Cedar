#include "layer.h"

#include "ielement.h"
#include "../math.h"



namespace Cedar::GUI
{
    void Layer::render(Size2D<int> windowSize, const Rectangle<int>& parentGlobalBounds, bool forceUpdate)
    {
        bool update = forceUpdate || updated() != UpdateType::not_updated;
        Rectangle<int> bounds = globalBounds(parentGlobalBounds);

        // TODO: Figure out why a helper function is necessary instead of calling
        //       child->render directly because I forgor.
        for (const auto& child : children())
            renderChild(child, windowSize, bounds, update);

        markAsUpdated(UpdateType::not_updated);
    }
}