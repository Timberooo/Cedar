#include "box.h"

#include "ielement.h"
#include "../color.h"
#include "../math.h"



namespace Cedar::GUI
{
    Array2D<Color> Box::draw(Size2D<std::size_t> bufferSize) const
    {
        Array2D<Color> output(bufferSize, getBackgroundColor());
        return output;
    }
}