#ifndef CEDAR_GUI_BOX_H
#define CEDAR_GUI_BOX_H

#include "ielement.h"
#include "../color.h"
#include "../math.h"



namespace Cedar::GUI
{
    class Box;



    class Box final : public IDrawableElement
    {
    private:

        Array2D<Color> draw(Size2D<std::size_t> bufferSize) const final;
    };
}

#endif // CEDAR_GUI_BOX_H