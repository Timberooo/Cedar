//
// Point data structures.
//
// Point structs must adhere to the following format and rules:
// * Structs must be named "PointXD" where "X" is the number of dimensions the struct
//   represents.
// * Must be a template that takes a generic type T, where T is the type of the member
//   variables. All member variables must be the same type.
// * Member variables should be single letter names. Common naming conventions in
//   mathematics (example: x, y, z) are encouraged.
// * The equality and inequality operators must be defined to compare each of the
//   struct's members.
// * No methods are allowed aside from equality operators and default members; points
//   should be strictly POD.
//

#ifndef CEDAR_MATH_POINT_H
#define CEDAR_MATH_POINT_H

namespace Cedar
{
    template <typename T>
    struct Point2D;

    template <typename T>
    struct Point3D;



    template <typename T>
    struct Point2D
    {
        T x;
        T y;

        inline bool operator==(const Point2D<T>& other) const {
            return x == other.x && y == other.y;
        }

        inline bool operator!=(const Point2D<T>& other) const {
            return !operator==(other);
        }
    };



    template <typename T>
    struct Point3D
    {
        T x;
        T y;
        T z;

        inline bool operator==(const Point3D<T>& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        inline bool operator!=(const Point3D<T>& other) const {
            return !operator==(other);
        }
    };
}

#endif // CEDAR_MATH_POINT_H