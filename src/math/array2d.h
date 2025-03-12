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

        inline Array2D(std::size_t width, std::size_t height);

        inline Array2D(Size2D<std::size_t> initSize, const T& value);

        inline Array2D(std::size_t width, std::size_t height, const T& value);


        inline bool empty() const;

        inline Size2D<std::size_t> size() const;


        T& at(Point2D<std::size_t> pos);

        inline T& at(std::size_t x, std::size_t y);

        const T& at(Point2D<std::size_t> pos) const;

        inline const T& at(std::size_t x, std::size_t y) const;


        void resize(Size2D<std::size_t> newSize);

        inline void resize(std::size_t width, std::size_t height);

        void resize(Size2D<std::size_t> newSize, const T& value);

        inline void resize(std::size_t width, std::size_t height, const T& value);

        inline void clear();


        void insert(const Array2D& other, Point2D<std::size_t> pos);

        void insert(const Array2D& other, Point2D<std::size_t> pos, Point2D<std::size_t> otherPos);

        //void insert(const Array2D& other, Point2D<std::size_t> pos, Rectangle<std::size_t> otherRect);

    private:

        std::vector<T>      m_data;
        Size2D<std::size_t> m_size;


        static inline std::size_t index(Point2D<std::size_t> pos, std::size_t stride);


        inline std::size_t stride() const;

        inline bool inBounds(Point2D<std::size_t> pos) const;
    };



    template <typename T>
    inline Array2D<T>::Array2D() : Array2D(0, 0) {}

    template <typename T>
    inline Array2D<T>::Array2D(Size2D<std::size_t> initSize) :
        m_data(initSize.width * initSize.height), m_size(initSize) {}

    template <typename T>
    inline Array2D<T>::Array2D(std::size_t width, std::size_t height) :
        Array2D(Size2D<std::size_t>{ width, height }) {}

    template <typename T>
    inline Array2D<T>::Array2D(Size2D<std::size_t> initSize, const T& value) :
        m_data(initSize.width * initSize.height, value), m_size(initSize) {}

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
    T& Array2D<T>::at(Point2D<std::size_t> pos)
    {
        if (!inBounds(pos))
            throw std::out_of_range("Position was outside the 2D array");

        return m_data.at(index(pos, stride()));
    }

    template <typename T>
    inline T& Array2D<T>::at(std::size_t x, std::size_t y) {
        return at({ x, y });
    }

    template <typename T>
    const T& Array2D<T>::at(Point2D<std::size_t> pos) const
    {
        if (!inBounds(pos))
            throw std::out_of_range("Position was outside the 2D array");

        return m_data.at(index(pos, stride()));
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

        std::vector<T> newData(newSize.width * newSize.height);
        std::size_t newStride = newSize.width;

        for (std::size_t y = 0; y < ceiling(size().height, newSize.height); y++)
            for (std::size_t x = 0; x < ceiling(size().width, newSize.width); x++)
                newData.at(index({ x, y }, newStride)) = m_data.at(index({ x, y }, stride()));

        m_data = newData;
        m_size = newSize;
    }

    template <typename T>
    inline void Array2D<T>::resize(std::size_t width, std::size_t height) {
        resize({ width, height });
    }

    template <typename T>
    void Array2D<T>::resize(Size2D<std::size_t> newSize, const T& value)
    {
        if (newSize == size())
            return;

        std::vector<T> newData(newSize.width * newSize.height, value);
        std::size_t newStride = newSize.width;

        for (std::size_t y = 0; y < ceiling(size().height, newSize.height); y++)
            for (std::size_t x = 0; x < ceiling(size().width, newSize.width); x++)
                newData.at(index({ x, y }, newStride)) = m_data.at(index({ x, y }, stride()));

        m_data = newData;
        m_size = newSize;
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
    void Array2D<T>::insert(const Array2D<T>& other, Point2D<std::size_t> pos)
    {
        if (!inBounds(pos))
            return;

        Size2D<std::size_t> updateSize;
        updateSize.width = size().width - pos.x;
        updateSize.height = size().height - pos.y;

        Size2D<std::size_t> copySize;
        copySize.width = ceiling(updateSize.width, other.size().width);
        copySize.height = ceiling(updateSize.height, other.size().height);

        for (std::size_t y = 0; y < copySize.height; y++)
            for (std::size_t x = 0; x < copySize.width; x++)
                at(x + pos.x, y + pos.y) = other.at(x, y);
    }

    template <typename T>
    void Array2D<T>::insert(const Array2D<T>& other, Point2D<std::size_t> pos, Point2D<std::size_t> otherPos)
    {
        if (!inBounds(pos) || !other.inBounds(otherPos))
            return;

        Size2D<std::size_t> updateSize;
        updateSize.width = size().width - pos.x;
        updateSize.height = size().height - pos.y;

        Size2D<std::size_t> otherUpdateSize;
        otherUpdateSize.width = other.size().width - otherPos.x;
        otherUpdateSize.height = other.size().height - otherPos.y;

        Size2D<std::size_t> copySize;
        copySize.width = ceiling(updateSize.width, otherUpdateSize.width);
        copySize.height = ceiling(updateSize.height, otherUpdateSize.height);

        for (std::size_t y = 0; y < copySize.height; y++)
            for (std::size_t x = 0; x < copySize.width; x++)
                at(x + pos.x, y + pos.y) = other.at(x + otherPos.x, y + otherPos.y);
    }



    template <typename T>
    inline std::size_t Array2D<T>::index(Point2D<std::size_t> pos, std::size_t stride) {
        return pos.x + (pos.y * stride);
    }



    template <typename T>
    inline std::size_t Array2D<T>::stride() const {
        return size().width;
    }



    template <typename T>
    inline bool Array2D<T>::inBounds(Point2D<std::size_t> pos) const {
        return pos.x < size().width && pos.y < size().height;
    }
}

#endif // CEDAR_MATH_ARRAY2D_H