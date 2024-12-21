#include "Utils.h"

#include <memory>

vg::VoidArray::VoidArray(size_t size)
{
    _v = malloc(size);
    if (!_v) throw std::bad_alloc();
}

vg::VoidArray::VoidArray(void* raw)
    : _v(raw)
{
}

vg::VoidArray::VoidArray(VoidArray&& other) noexcept
    : _v(other._v)
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
    }
    return *this;
}

vg::VoidArray::~VoidArray()
{
    free(_v);
}
