#pragma once

#include "Errors.h"

namespace vg
{
	// TODO clone()

	class VoidArray
	{
		void* _v = nullptr;
		size_t _size;

	public:
		VoidArray(size_t size = 0);
		VoidArray(const VoidArray&) = delete;
		VoidArray(VoidArray&&) noexcept;
		VoidArray& operator=(VoidArray&&) noexcept;
		~VoidArray();

		size_t size() const { return _size; }
		void resize(size_t size);

		operator const void* () const { return _v; }
		operator void* () { return _v; }
		const void* operator[](size_t offset) const;
		void* operator[](size_t offset);

		template<typename Type>
		const Type& ref(size_t offset_bytes) const
		{
			if (offset_bytes + sizeof(Type) > _size)
				throw offset_out_of_range(_size, offset_bytes, sizeof(Type));
			return *reinterpret_cast<Type*>((char*)_v + offset_bytes);
		}

		template<typename Type>
		Type& ref(size_t offset_bytes)
		{
			if (offset_bytes + sizeof(Type) > _size)
				throw offset_out_of_range(_size, offset_bytes, sizeof(Type));
			return *reinterpret_cast<Type*>((char*)_v + offset_bytes);
		}

		template<typename Type>
		Type val(size_t offset_bytes) const
		{
			if (offset_bytes + sizeof(Type) > _size)
				throw offset_out_of_range(_size, offset_bytes, sizeof(Type));
			return *reinterpret_cast<Type*>((char*)_v + offset_bytes);
		}
	};
}
