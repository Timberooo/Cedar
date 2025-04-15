#include "box.h"



namespace Cedar::GUI
{
    Array2D<Box::Cell> Box::render(Size2D<std::size_t> size) const
    {
        Array2D<Cell> output(size, { getBackgroundColor(), ' ' });
        return output;
    }
}