//
// Rectangle data structure.
//

#ifndef CEDAR_MATH_RECTANGLE_H
#define CEDAR_MATH_RECTANGLE_H

#include "point.h"
#include "size.h"



namespace Cedar
{
    template <typename T>
    struct Rectangle;



    template <typename T>
    struct Rectangle
    {
        Point2D<T> position;
        Size2D<T>  size;

        
        inline bool operator==(const Rectangle<T>& other) const {
            return position == other.position && size == other.size;
        }

        inline bool operator!=(const Rectangle<T>& other) const {
            return !operator!=(other);
        }


        inline Point2D<T> bottomRight() const {
            return { position.x + size.width, position.y + size.height };
        }
    };
}

#endif // CEDAR_MATH_RECTANGLE_H