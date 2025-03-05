//
// Array2D.
//

#ifndef CEDAR_MATH_ARRAY2D_H
#define CEDAR_MATH_ARRAY2D_H

#include "point.h"
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

        inline Array2D() : Array2D(0, 0) {}

        inline Array2D(Size2D<std::size_t> initSize);

        inline Array2D(std::size_t width, std::size_t height) : Array2D(Size2D<std::size_t>{ width, height }) {}

        inline Array2D(Size2D<std::size_t> initSize, const T& value);

        inline Array2D(std::size_t width, std::size_t height, const T& value) : Array2D(Size2D<std::size_t>{ width, height }, value) {}


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

    private:

        std::vector<T>      m_data;
        Size2D<std::size_t> m_size;


        static inline std::size_t index(Point2D<std::size_t> pos, std::size_t stride);


        inline std::size_t stride() const;

        void copyDataToNewData(std::vector<T>& newData, Size2D<std::size_t> newSize);
    };



    template <typename T>
    inline Array2D<T>::Array2D(Size2D<std::size_t> initSize) : m_size(initSize) {
        m_data = std::vector<T>(initSize.width * initSize.height);
    }

    template <typename T>
    inline Array2D<T>::Array2D(Size2D<std::size_t> initSize, const T& value) : m_size(initSize) {
        m_data = std::vector<T>(initSize.width * initSize.height, value);
    }



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
        if (pos.x >= size().width || pos.y >= size().height)
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
        if (pos.x >= size().width || pos.y >= size().height)
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
        copyDataToNewData(newData, newSize);
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
        copyDataToNewData(newData, newSize);
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
    inline std::size_t Array2D<T>::index(Point2D<std::size_t> pos, std::size_t stride) {
        return pos.x + (pos.y * stride);
    }



    template <typename T>
    inline std::size_t Array2D<T>::stride() const {
        return size().width;
    }



    template <typename T>
    void Array2D<T>::copyDataToNewData(std::vector<T>& newData, Size2D<std::size_t> newSize)
    {
        std::size_t newStride = newSize.width;

        for (std::size_t y = 0; y < size().height && y < newSize.height; y++)
            for (std::size_t x = 0; x < size().width && x < newSize.width; x++)
                newData.at(index({ x, y }, newStride)) = m_data.at(index({ x, y }, stride()));
    }
}

#endif // CEDAR_MATH_ARRAY2D_H