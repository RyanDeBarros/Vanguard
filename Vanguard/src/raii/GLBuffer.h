#pragma once

#include "Vendor.h"

namespace vg
{
	class GLBuffer
	{
		GLuint _b;

	public:
		GLBuffer();
		GLBuffer(const GLBuffer&) = delete;
		GLBuffer(GLBuffer&&) noexcept;
		GLBuffer& operator=(GLBuffer&&) noexcept;
		~GLBuffer();

		operator GLuint () const { return _b; }
	};

	class GLBufferBlock
	{
		GLuint* _b;
		GLsizei count;

	public:
		GLBufferBlock(GLsizei count);
		GLBufferBlock(const GLBufferBlock&) = delete;
		GLBufferBlock(GLBufferBlock&&) noexcept;
		GLBufferBlock& operator=(GLBufferBlock&&) noexcept;
		~GLBufferBlock();

		GLuint operator[](GLsizei i) const;
	};

	enum class BufferMutableUsage
	{
		STREAM_DRAW = GL_STREAM_DRAW,
		STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY
	};

	namespace BufferImmutableUsage
	{
		enum
		{
			DYNAMIC_STORAGE = GL_DYNAMIC_STORAGE_BIT,
			MAP_READ = GL_MAP_READ_BIT,
			MAP_WRITE = GL_MAP_WRITE_BIT,
			MAP_PERSISTENT = GL_MAP_PERSISTENT_BIT,
			MAP_COHERENT = GL_MAP_COHERENT_BIT,
			CLIENT_STORAGE = GL_CLIENT_STORAGE_BIT
		};
	}

	namespace vertex_buffer
	{
		extern void bind(GLuint vb);
		extern void unbind();
		extern void init_immutable(GLuint vb, GLsizeiptr size, const void* data = nullptr, int usage = BufferImmutableUsage::DYNAMIC_STORAGE);
		extern void init_mutable(GLuint vb, GLsizeiptr size, const void* data, BufferMutableUsage usage = BufferMutableUsage::DYNAMIC_DRAW);
		extern void subsend(GLuint vb, GLintptr offset, GLsizeiptr size, const void* data);
		extern void map(GLuint vb, void* data, size_t size);
		extern void submap(GLuint vb, GLintptr offset, GLsizeiptr length, void* data);
	}

	namespace index_buffer
	{
		extern void bind(GLuint ib);
		extern void unbind();
		extern void init_immutable(GLuint ib, GLsizeiptr size, const void* data = nullptr, int usage = BufferImmutableUsage::DYNAMIC_STORAGE);
		extern void init_mutable(GLuint ib, GLsizeiptr size, const void* data, BufferMutableUsage usage = BufferMutableUsage::DYNAMIC_DRAW);
		extern void subsend(GLuint ib, GLintptr offset, GLsizeiptr size, const void* data);
		extern void map(GLuint ib, void* data, size_t size);
		extern void submap(GLuint ib, GLintptr offset, GLsizeiptr length, void* data);
	}

	extern void copy_gl_buffer(GLuint vb_src, GLuint vb_dst, GLintptr offset_src, GLintptr offset_dst, GLsizeiptr size);

	class VertexArray
	{
		// TODO
	};

	// LATER SharedVertexArray that uses shared_ptr<VertexBuffer> and shared_ptr<IndexBuffer>
}
