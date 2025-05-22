//
// Common math functions.
//
// Any math functions that do not have an equivalent or an easy to understand/use
// equivalent in the C++ standard library and ae too generic to warrant their own header
// file should go here.
//
// Including other headers within this directory in this file is forbidden, as that poses
// a risk for creating circular dependencies.
//

#ifndef CEDAR_MATH_MATH_COMMON_H
#define CEDAR_MATH_MATH_COMMON_H

namespace Cedar
{
    template <typename T>
    inline T floor(T value, T min);

    template <typename T>
    inline T ceiling(T value, T max);

    template <typename T>
    inline T clamp(T value, T min, T max);



    template <typename T>
    inline T floor(T value, T min)
    {
        return value < min ? min : value;
    }



    template <typename T>
    inline T ceiling(T value, T max)
    {
        return value > max ? max : value;
    }



    template <typename T>
    inline T clamp(T value, T min, T max)
    {
        if (value < min)
            value = min;
        else if (value > max)
            value = max;

        return value;
    }
}

#endif // CEDAR_MATH_MATH_COMMON_H