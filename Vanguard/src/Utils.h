#pragma once

namespace vg
{
	class VoidArray
	{
		void* _v = nullptr;

	public:
		VoidArray(size_t size);
		VoidArray(void* raw);
		VoidArray(const VoidArray&) = delete;
		VoidArray(VoidArray&&) noexcept;
		VoidArray& operator=(VoidArray&&) noexcept;
		~VoidArray();

		operator const void* () const { return _v; }
		operator void* () { return _v; }
	};
}
