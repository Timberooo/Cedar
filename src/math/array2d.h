//
// Array2D.
//

#ifndef CEDAR_MATH_ARRAY2D_H
#define CEDAR_MATH_ARRAY2D_H

#include "math_common.h"
#include "point.h"
#include "rectangle.h"
#include "size.h"

#include <cstddef>
#include <vector>
#include <stdexcept>



namespace Cedar
{
    template <typename T>
    class Array2D;



    template <typename T>
    class Array2D
    {
    public:

        inline Array2D();

        inline Array2D(Size2D<std::size_t> initSize);

        inline Array2D(Size2D<std::size_t> initSize, const T& value);

        inline Array2D(std::size_t width, std::size_t height);

        inline Array2D(std::size_t width, std::size_t height, const T& value);


        inline bool empty() const;

        inline Size2D<std::size_t> size() const;

        inline bool inBounds(Point2D<std::size_t> pos) const;

        inline bool inBounds(std::size_t x, std::size_t y) const;


        T& at(Point2D<std::size_t> pos);

        const T& at(Point2D<std::size_t> pos) const;

        inline T& at(std::size_t x, std::size_t y);

        inline const T& at(std::size_t x, std::size_t y) const;


        void resize(Size2D<std::size_t> newSize);

        void resize(Size2D<std::size_t> newSize, const T& value);

        inline void resize(std::size_t width, std::size_t height);

        inline void resize(std::size_t width, std::size_t height, const T& value);

        inline void clear();


        template <typename F>
        inline void foreach(F func);

        template <typename F>
        inline void foreach(F func) const;

        template <typename F>
        void foreach(Point2D<std::size_t> start, Size2D<std::size_t> size, F func);

        template <typename F>
        void foreach(Point2D<std::size_t> start, Size2D<std::size_t> size, F func) const;

        template <typename F>
        inline void foreach(std::size_t xStart, std::size_t yStart, std::size_t width, std::size_t height, F func);

        template <typename F>
        inline void foreach(std::size_t xStart, std::size_t yStart, std::size_t width, std::size_t height, F func) const;

    private:

        std::vector<T>      m_data;
        Size2D<std::size_t> m_size;


        inline std::size_t stride() const;

        inline std::size_t index(Point2D<std::size_t> pos) const;

        inline std::size_t index(std::size_t x, std::size_t y) const;


        void updateResizedData(Array2D<T>& newArray);
    };



    template <typename T>
    inline Array2D<T>::Array2D() : Array2D(0, 0) {}

    template <typename T>
    inline Array2D<T>::Array2D(Size2D<std::size_t> initSize) :
        m_data(initSize.width * initSize.height), m_size(initSize) {}

    template <typename T>
    inline Array2D<T>::Array2D(Size2D<std::size_t> initSize, const T& value) :
        m_data(initSize.width * initSize.height, value), m_size(initSize) {}

    template <typename T>
    inline Array2D<T>::Array2D(std::size_t width, std::size_t height) :
        Array2D(Size2D<std::size_t>{ width, height }) {}

    template <typename T>
    inline Array2D<T>::Array2D(std::size_t width, std::size_t height, const T& value) :
        Array2D(Size2D<std::size_t>{ width, height }, value) {}



    template <typename T>
    inline bool Array2D<T>::empty() const {
        return size().width == 0; // Width and height should both be 0 if array is empty
    }



    template <typename T>
    inline Size2D<std::size_t> Array2D<T>::size() const {
        return m_size;
    }



    template <typename T>
    inline bool Array2D<T>::inBounds(Point2D<std::size_t> pos) const {
        return (pos.x < size().width && pos.y < size().height);
    }

    template <typename T>
    inline bool Array2D<T>::inBounds(std::size_t x, std::size_t y) const {
        return inBounds({ x, y });
    }



    template <typename T>
    T& Array2D<T>::at(Point2D<std::size_t> pos)
    {
        if (inBounds(pos))
            return m_data.at(index(pos));
        else
            throw std::out_of_range("Position was outside the 2D array");
    }

    template <typename T>
    const T& Array2D<T>::at(Point2D<std::size_t> pos) const
    {
        if (inBounds(pos))
            return m_data.at(index(pos));
        else
            throw std::out_of_range("Position was outside the 2D array");
    }

    template <typename T>
    inline T& Array2D<T>::at(std::size_t x, std::size_t y) {
        return at({ x, y });
    }

    template <typename T>
    inline const T& Array2D<T>::at(std::size_t x, std::size_t y) const {
        return at({ x, y });
    }



    template <typename T>
    void Array2D<T>::resize(Size2D<std::size_t> newSize)
    {
        if (newSize == size())
            return;

        Array2D<T> newArray(newSize);
        updateResizedData(newArray);
    }

    template <typename T>
    void Array2D<T>::resize(Size2D<std::size_t> newSize, const T& value)
    {
        if (newSize == size())
            return;

        Array2D<T> newArray(newSize, value);
        updateResizedData(newArray);
    }

    template <typename T>
    inline void Array2D<T>::resize(std::size_t width, std::size_t height) {
        resize({ width, height });
    }

    template <typename T>
    inline void Array2D<T>::resize(std::size_t width, std::size_t height, const T& value) {
        resize({ width, height }, value);
    }



    template <typename T>
    inline void Array2D<T>::clear() {
        m_data.clear();
        m_size = { 0, 0 };
    }



    template <typename T>
    template <typename F>
    inline void Array2D<T>::foreach(F func) {
        foreach(0, 0, size().width, size().height, func);
    }

    template <typename T>
    template <typename F>
    inline void Array2D<T>::foreach(F func) const {
        foreach(0, 0, size().width, size().height, func);
    }

    template <typename T>
    template <typename F>
    void Array2D<T>::foreach(Point2D<std::size_t> start, Size2D<std::size_t> size, F func)
    {
        for (std::size_t y = start.y; y < size.height; y++)
            for (std::size_t x = start.x; x < size.width; x++)
                func(Point2D<std::size_t>{ x, y });
    }

    template <typename T>
    template <typename F>
    void Array2D<T>::foreach(Point2D<std::size_t> start, Size2D<std::size_t> size, F func) const
    {
        for (std::size_t y = start.y; y < size.height; y++)
            for (std::size_t x = start.x; x < size.width; x++)
                func(Point2D<std::size_t>{ x, y });
    }

    template <typename T>
    template <typename F>
    inline void Array2D<T>::foreach(std::size_t xStart, std::size_t yStart, std::size_t width, std::size_t height, F func) {
        foreach({ xStart, yStart }, { width, height }, func);
    }

    template <typename T>
    template <typename F>
    inline void Array2D<T>::foreach(std::size_t xStart, std::size_t yStart, std::size_t width, std::size_t height, F func) const {
        foreach({ xStart, yStart }, { width, height }, func);
    }



    template <typename T>
    inline std::size_t Array2D<T>::stride() const {
        return size().width;
    }



    template <typename T>
    inline std::size_t Array2D<T>::index(Point2D<std::size_t> pos) const {
        return pos.x + (pos.y * stride());
    }

    template <typename T>
    inline std::size_t Array2D<T>::index(std::size_t x, std::size_t y) const {
        return index({ x, y });
    }



    template <typename T>
    void Array2D<T>::updateResizedData(Array2D<T>& newArray)
    {
        foreach(0, 0,
                ceiling(size().width, newArray.size().width),
                ceiling(size().height, newArray.size().height),
                [&](Point2D<std::size_t> pos)
        {
            newArray.m_data.at(newArray.index(pos.x, pos.y)) = m_data.at(index(pos.x, pos.y));
        });

        *this = newArray;
    }
}

#endif // CEDAR_MATH_ARRAY2D_H