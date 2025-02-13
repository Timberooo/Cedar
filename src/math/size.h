//
// Size data structures.
//
// Size structs must adhere to the following format and rules:
// * Dimensions must be named "SizeXD" where "X" is the number of dimensions the struct
//   represents.
// * Must be a template that takes a generic type T, where T is the type of the member
//   variables. All member variables must be the same type.
// * Member variables should be common names for values in that dimension (example:
//   length, width, height for 3D).
// * The equality and inequality operators must be defined to compare each of the
//   struct's members.
// * No methods are allowed aside from equality operators and default members; sizes
//   should be strictly POD.
//

#ifndef CEDAR_MATH_SIZE_H
#define CEDAR_MATH_SIZE_H

namespace Cedar
{
    template <typename T>
    struct Size2D;

    template <typename T>
    struct Size3D;



    template <typename T>
    struct Size2D
    {
        T width;
        T height;

        inline bool operator==(const Size2D<T>& other) const {
            return width == other.width && height == other.height;
        }

        inline bool operator!=(const Size2D<T>& other) const {
            return !operator==(other);
        }
    };



    template <typename T>
    struct Size3D
    {
        T width;
        T height;
        T length;

        inline bool operator==(const Size3D<T>& other) const {
            return width == other.width && height == other.height && length == other.length;
        }

        inline bool operator!=(const Size3D<T>& other) const {
            return !operator==(other);
        }
    };
}

#endif // CEDAR_MATH_SIZE_H