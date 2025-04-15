#ifndef CEDAR_GUI_BOX_H
#define CEDAR_GUI_BOX_H

#include "ielement.h"



namespace Cedar::GUI
{
    class Box;



    class Box : public IElement
    {
    public:

        Array2D<Cell> render(Size2D<std::size_t> size) const final;
    };
}

#endif // CEDAR_GUI_BOX_H