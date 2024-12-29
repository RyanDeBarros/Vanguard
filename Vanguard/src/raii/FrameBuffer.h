#pragma once

#include "Vendor.h"

namespace vg
{
	namespace ids
	{
		class FrameBuffer
		{
			GLuint id;

		public:
			explicit FrameBuffer(GLuint id = 0) : id(id) {}
			operator GLuint () const { return id; }
		};
	}

	namespace raii
	{
		class FrameBuffer
		{
			ids::FrameBuffer _f;

		public:
			FrameBuffer();
			FrameBuffer(const FrameBuffer&) = delete;
			FrameBuffer(FrameBuffer&&) noexcept;
			FrameBuffer& operator=(FrameBuffer&&) noexcept;
			~FrameBuffer();

			operator ids::FrameBuffer() const { return _f; }
		};

		class FrameBufferBlock
		{
			ids::FrameBuffer* _fs = nullptr;
			GLuint count;

		public:
			FrameBufferBlock(GLuint count);
			FrameBufferBlock(const FrameBufferBlock&) = delete;
			FrameBufferBlock(FrameBufferBlock&&) noexcept;
			FrameBufferBlock& operator=(FrameBufferBlock&&) noexcept;
			~FrameBufferBlock();

			ids::FrameBuffer operator[](GLuint i) const;
			GLuint get_count() const { return count; }
		};
	}

	enum class FrameBufferUsage
	{
		DUAL,
		DRAW,
		READ
	};
	
	extern void bind_framebuffer(ids::FrameBuffer fb, FrameBufferUsage usage = FrameBufferUsage::DUAL);
	extern void unbind_framebuffer(FrameBufferUsage usage = FrameBufferUsage::DUAL);
}
