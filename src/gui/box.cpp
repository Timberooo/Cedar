#include "box.h"



namespace Cedar::GUI
{
    Array2D<Color> Box::draw(Size2D<std::size_t> size) const
    {
        Array2D<Color> output(size, { getBackgroundColor() });
        return output;
    }
}