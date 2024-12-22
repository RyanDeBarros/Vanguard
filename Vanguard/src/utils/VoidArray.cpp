#include "VoidArray.h"

#include <memory>

vg::VoidArray::VoidArray(size_t size)
    : _size(size)
{
    _v = malloc(_size);
    if (!_v) throw std::bad_alloc();
}

vg::VoidArray::VoidArray(VoidArray&& other) noexcept
    : _v(other._v), _size(other._size)
{
    other._v = nullptr;
}

vg::VoidArray& vg::VoidArray::operator=(VoidArray&& other) noexcept
{
    if (this != &other)
    {
        free(_v);
        _v = other._v;
        other._v = nullptr;
        _size = other._size;
    }
    return *this;
}

vg::VoidArray::~VoidArray()
{
    free(_v);
}

void vg::VoidArray::resize(size_t size)
{
    void* r = realloc(_v, size);
    if (!r)
        throw std::bad_alloc();
    _v = r;
    _size = size;
}

const void* vg::VoidArray::operator[](size_t offset) const
{
    if (offset > _size)
        throw offset_out_of_range(_size, offset, 1);
    return (char*)_v + offset;
}

void* vg::VoidArray::operator[](size_t offset)
{
    if (offset > _size)
        throw offset_out_of_range(_size, offset, 1);
    return (char*)_v + offset;
}
