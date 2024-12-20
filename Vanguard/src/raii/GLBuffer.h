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
		GLuint* _b = nullptr;
		GLsizei count;

	public:
		GLBufferBlock(GLsizei count);
		GLBufferBlock(const GLBufferBlock&) = delete;
		GLBufferBlock(GLBufferBlock&&) noexcept;
		GLBufferBlock& operator=(GLBufferBlock&&) noexcept;
		~GLBufferBlock();

		GLuint operator[](GLsizei i) const;
	};

	enum class IndexDataType
	{
		UBYTE = GL_UNSIGNED_BYTE,
		USHORT = GL_UNSIGNED_SHORT,
		UINT = GL_UNSIGNED_INT
	};

	extern GLintptr index_data_type_size(IndexDataType idt);

	class VertexArray
	{
		GLuint _vao;
		IndexDataType idt;
		GLBufferBlock block;

	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;
		~VertexArray();

		operator GLuint () const { return _vao; }
		GLuint vb() const;
		GLuint ib() const;
		IndexDataType index_data_type() const { return idt; }
		IndexDataType& index_data_type() { return idt; }
		void bind() const;
	};

	class VertexArrayBlock
	{
		GLuint* _vaos = nullptr;
		IndexDataType* idts = nullptr;
		GLsizei count;
		GLBufferBlock block;

	public:
		VertexArrayBlock(GLsizei count);
		VertexArrayBlock(const VertexArrayBlock&) = delete;
		VertexArrayBlock(VertexArrayBlock&&) noexcept;
		VertexArrayBlock& operator=(VertexArrayBlock&&) noexcept;
		~VertexArrayBlock();

		GLuint operator[](GLsizei i) const;
		GLuint vb(GLsizei i) const;
		GLuint ib(GLsizei i) const;
		IndexDataType index_data_type(GLsizei i) const;
		IndexDataType& index_data_type(GLsizei i);
		void bind(GLsizei i) const;
	};

	extern void unbind_vertex_array();

	// LATER SharedVertexArray that uses shared_ptr<VertexBuffer> and shared_ptr<IndexBuffer>

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

	enum class DrawMode
	{
		POINTS = GL_POINTS,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		LINES = GL_LINES,
		LINE_STRIP_ADJENCY = GL_LINE_STRIP_ADJACENCY,
		LINES_ADJENCY = GL_LINES_ADJACENCY,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PATCHES = GL_PATCHES
	};

	namespace draw
	{
		extern void arrays(GLuint vb, DrawMode mode, GLint first, GLsizei count);
		extern void elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset);
		extern void elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset);
		extern void element_range(const VertexArray& vao, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset);
		extern void element_range(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset);

		namespace instanced
		{
			extern void arrays(GLuint vb, DrawMode mode, GLint first, GLsizei count, GLsizei primcount);
			extern void elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount);
			extern void elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount);

			extern void arrays(GLuint vb, DrawMode mode, GLint first, GLsizei count, GLsizei primcount, GLuint primoffset);
			extern void elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLuint primoffset);
			extern void elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLuint primoffset);

			namespace base_vertex
			{
				extern void elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex);
				extern void elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex);

				extern void elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex, GLuint primoffset);
				extern void elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex, GLuint primoffset);
			}
		}

		namespace base_vertex
		{
			extern void elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLint base_vertex);
			extern void elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLint base_vertex);
			extern void element_range(const VertexArray& vao, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset, GLint base_vertex);
			extern void element_range(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset, GLint base_vertex);
		}
	}
}
