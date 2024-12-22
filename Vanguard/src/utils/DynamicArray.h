#pragma once

#include <stdexcept>
#include <string>

namespace vg
{
	// LATER clone(), use malloc/free/realloc with emplace() function. Utilize throughout project

	template<typename Type>
	class DynamicArray
	{
		Type* _arr = nullptr;
		size_t _size = 0;

	public:
		DynamicArray(size_t size = 0) : _size(size) { _arr = new Type[_size]; }
		template<typename T>
		DynamicArray(const std::initializer_list<T>& objs)
			: _size(objs.size())
		{
			_arr = new Type[_size];
			auto iter = objs.begin();
			for (size_t i = 0; i < _size; ++i)
				_arr[i] = *iter++;
		}
		DynamicArray(const DynamicArray<Type>&) = delete;
		DynamicArray(DynamicArray<Type>&& other) noexcept : _arr(other._arr), _size(other._size) { other._arr = nullptr; other._size = 0; }
		DynamicArray<Type>& operator=(DynamicArray<Type>&& other) noexcept
		{
			if (this != &other)
			{
				delete[] _arr;
				_arr = other._arr;
				other._arr = nullptr;
				_size = other._size;
				other._size = 0;
			}
			return *this;
		}

		size_t size() const { return _size; }
		void resize(size_t size)
		{
			if (size == _size)
				return;
			Type* new_arr = new Type[size];
			memcpy(new_arr, _arr, std::min(_size, size));
			delete[] _arr;
			_arr = new_arr;
		}

		operator const Type* () const { return _arr; }
		operator Type* () { return _arr; }
		const Type& operator[](size_t i) const
		{
			if (i >= _size)
				throw std::out_of_range("Index " + std::to_string(i) + " out of range for DynamicArray of size " + std::to_string(_size));
			return _arr[i];
		}
		Type& operator[](size_t i)
		{
			if (i >= _size)
				throw std::out_of_range("Index " + std::to_string(i) + " out of range for DynamicArray of size " + std::to_string(_size));
			return _arr[i];
		}
		Type val(size_t i) const
		{
			if (i >= _size)
				throw std::out_of_range("Index " + std::to_string(i) + " out of range for DynamicArray of size " + std::to_string(_size));
			return _arr[i];
		}
	};
}
