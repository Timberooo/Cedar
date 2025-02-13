//
// Vector data structures.
//
// Vector structs must adhere to the following format and rules:
// * Structs must be named "VectorXD" where "X" is the number of dimensions the struct
//   represents.
// * Must be a template that takes a generic type T, where T is the type of the member
//   variables. All member variables must be the same type.
// * Member variables should be single letter names. Common naming conventions in
//   mathematics (example: x, y, z, w) are encouraged.
// * The equality and inequality operators must be defined to compare each of the
//   struct's members.
// * No methods are allowed aside from equality operators and default members; vectors
//   should be strictly POD.
//

#ifndef CEDAR_MATH_VECTOR_H
#define CEDAR_MATH_VECTOR_H

namespace Cedar
{
    template <typename T>
    struct Vector2D;

    template <typename T>
    struct Vector3D;



    template <typename T>
    struct Vector2D
    {
        T x;
        T y;

        inline bool operator==(const Vector2D<T>& other) const {
            return x == other.x && y == other.y;
        }

        inline bool operator!=(const Vector2D<T>& other) const {
            return !operator==(other);
        }
    };



    template <typename T>
    struct Vector3D
    {
        T x;
        T y;
        T z;

        inline bool operator==(const Vector3D<T>& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        inline bool operator!=(const Vector3D<T>& other) const {
            return !operator==(other);
        }
    };
}

#endif // CEDAR_MATH_VECTOR_H