#ifndef CEDAR_GUI_BOX_H
#define CEDAR_GUI_BOX_H

#include "ielement.h"



namespace Cedar::GUI
{
    class Box;



    class Box : public IDrawableElement
    {
    private:

        Array2D<Color> draw(Size2D<std::size_t> size) const final;
    };
}

#endif // CEDAR_GUI_BOX_H