#pragma once

#include "Vendor.h"
#include "Utils.h"

// TODO clone() functions everywhere

namespace vg
{
	namespace ids
	{
		class GLBuffer
		{
			GLuint id;

		public:
			explicit GLBuffer(GLuint id = 0) : id(id) {}
			operator GLuint () const { return id; }
		};
	}

	namespace raii
	{
		class GLBuffer
		{
			using B = ids::GLBuffer;
			B _b;

		public:
			GLBuffer();
			GLBuffer(const GLBuffer&) = delete;
			GLBuffer(GLBuffer&&) noexcept;
			GLBuffer& operator=(GLBuffer&&) noexcept;
			~GLBuffer();

			operator ids::GLBuffer () const { return _b; }
		};

		class GLBufferBlock
		{
			using B = ids::GLBuffer;
			B* _bs = nullptr;
			GLuint count;

		public:
			GLBufferBlock(GLuint count);
			GLBufferBlock(const GLBufferBlock&) = delete;
			GLBufferBlock(GLBufferBlock&&) noexcept;
			GLBufferBlock& operator=(GLBufferBlock&&) noexcept;
			~GLBufferBlock();

			ids::GLBuffer operator[](GLuint i) const;
		};
	}
	namespace ids
	{
		class VertexArray
		{
			GLuint id;

		public:
			explicit VertexArray(GLuint id = 0) : id(id) {}
			operator GLuint () const { return id; }
		};
	}

	namespace raii
	{
		class VertexArray
		{
			using V = ids::VertexArray;
			V _vao;
			GLBufferBlock block;

		public:
			VertexArray();
			VertexArray(const VertexArray&) = delete;
			VertexArray(VertexArray&&) noexcept;
			VertexArray& operator=(VertexArray&&) noexcept;
			~VertexArray();

			operator ids::VertexArray () const { return _vao; }
			ids::GLBuffer vb() const;
			ids::GLBuffer ib() const;
			void bind() const;
		};

		class VertexArrayBlock
		{
			using V = ids::VertexArray;
			V* _vaos = nullptr;
			GLuint count;
			GLBufferBlock block;

		public:
			VertexArrayBlock(GLuint count);
			VertexArrayBlock(const VertexArrayBlock&) = delete;
			VertexArrayBlock(VertexArrayBlock&&) noexcept;
			VertexArrayBlock& operator=(VertexArrayBlock&&) noexcept;
			~VertexArrayBlock();

			ids::VertexArray operator[](GLuint i) const;
			ids::GLBuffer vb(GLuint i) const;
			ids::GLBuffer ib(GLuint i) const;
			void bind(GLuint i) const;
		};
	}

	extern void unbind_vertex_array();

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

	struct IndirectArraysCmd
	{
		GLuint vertex_count;
		GLuint instance_count;
		GLuint first_vertex;
		GLuint first_instance;
	};

	class GPUIndirectArrays
	{
		raii::GLBuffer b;

	public:
		GPUIndirectArrays();

		void bind() const;
		void draw(DrawMode mode) const;

		void send_vertex_count(GLuint vertex_count) const;
		void send_instance_count(GLuint instance_count) const;
		void send_first_vertex(GLuint first_vertex) const;
		void send_first_instance(GLuint first_instance) const;
		void send_cmd(IndirectArraysCmd cmd) const;
	};

	class GPUIndirectArraysBlock
	{
		raii::GLBuffer b;
		GLuint count;

	public:
		GPUIndirectArraysBlock(GLuint count);

		void bind() const;
		void draw(GLuint i, DrawMode mode) const;
		void multi_draw(DrawMode mode, GLuint first, GLuint count) const;

		GLuint get_count() const { return count; }

		void send_vertex_count(GLuint i, GLuint vertex_count) const;
		void send_instance_count(GLuint i, GLuint instance_count) const;
		void send_first_vertex(GLuint i, GLuint first_vertex) const;
		void send_first_instance(GLuint i, GLuint first_instance) const;
		void send_cmd(GLuint i, IndirectArraysCmd cmd) const;
		void send_cmds(GLuint first, GLuint count, IndirectArraysCmd* cmds) const;
	};

	class CPUIndirectArrays
	{
		GPUIndirectArrays g;

	public:
		IndirectArraysCmd cmd;

		CPUIndirectArrays() = default;

		void bind() const { g.bind(); }
		void draw(DrawMode mode) const { g.draw(mode); }

		void send_vertex_count() const { g.send_vertex_count(cmd.vertex_count); }
		void send_instance_count() const { g.send_instance_count(cmd.instance_count); }
		void send_first_vertex() const { g.send_first_vertex(cmd.first_vertex); }
		void send_first_instance() const { g.send_first_instance(cmd.first_instance); }
		void send_cmd() const { g.send_cmd(cmd); }
	};

	enum class IndexDataType
	{
		UBYTE = GL_UNSIGNED_BYTE,
		USHORT = GL_UNSIGNED_SHORT,
		UINT = GL_UNSIGNED_INT
	};

	extern GLintptr index_data_type_size(IndexDataType idt);

	struct IndirectElementsCmd
	{
		GLuint index_count;
		GLuint instance_count;
		GLuint first_index;
		GLuint base_vertex;
		GLuint first_instance;
	};

	class GPUIndirectElements
	{
		raii::GLBuffer b;

	public:
		GPUIndirectElements();

		void bind() const;
		void draw(DrawMode mode, IndexDataType idt) const;

		void send_index_count(GLuint index_count) const;
		void send_instance_count(GLuint instance_count) const;
		void send_first_index(GLuint first_index) const;
		void send_base_vertex(GLuint base_vertex) const;
		void send_first_instance(GLuint first_instance) const;
		void send_cmd(IndirectElementsCmd cmd) const;
	};

	class GPUIndirectElementsBlock
	{
		raii::GLBuffer b;
		GLuint count;

	public:
		GPUIndirectElementsBlock(GLuint count);

		void bind() const;
		void draw(GLuint i, DrawMode mode, IndexDataType idt) const;
		void multi_draw(DrawMode mode, GLuint first, GLuint count, IndexDataType idt) const;

		void send_index_count(GLuint i, GLuint index_count) const;
		void send_instance_count(GLuint i, GLuint instance_count) const;
		void send_first_index(GLuint i, GLuint first_index) const;
		void send_base_vertex(GLuint i, GLuint base_vertex) const;
		void send_first_instance(GLuint i, GLuint first_instance) const;
		void send_cmd(GLuint i, IndirectElementsCmd cmd) const;
		void send_cmds(GLuint first, GLuint count, IndirectElementsCmd* cmds) const;
	};

	class CPUIndirectElements
	{
		GPUIndirectElements g;

	public:
		IndirectElementsCmd cmd;
		IndexDataType idt;

		CPUIndirectElements() = default;

		void bind() const { g.bind(); }
		void draw(DrawMode mode) const { g.draw(mode, idt); }

		void send_index_count() const { g.send_index_count(cmd.index_count); }
		void send_instance_count() const { g.send_instance_count(cmd.instance_count); }
		void send_first_index() const { g.send_first_index(cmd.first_index); }
		void send_base_vertex() const { g.send_base_vertex(cmd.base_vertex); }
		void send_first_instance() const { g.send_first_instance(cmd.first_instance); }
		void send_cmd() const { g.send_cmd(cmd); }
	};

	enum class BufferTarget
	{
		VERTEX = GL_ARRAY_BUFFER,
		// GL_ATOMIC_COUNTER_BUFFER
		COPY_READ = GL_COPY_READ_BUFFER,
		COPY_WRITE = GL_COPY_WRITE_BUFFER,
		// GL_DISPATCH_INDIRECT_BUFFER
		DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
		INDEX = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_READ = GL_PIXEL_PACK_BUFFER,
		// GL_PIXEL_UNPACK_BUFFER
		QUERY = GL_QUERY_BUFFER,
		// GL_SHADER_STORAGE_BUFFER
		// GL_TEXTURE_BUFFER
		// GL_TRANSFORM_FEEDBACK_BUFFER
		UNIFORM = GL_UNIFORM_BUFFER,
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

	namespace buffers
	{
		extern void bind(ids::GLBuffer b, BufferTarget target);
		extern void unbind(BufferTarget target);
		extern void init_immutable(BufferTarget target, GLsizeiptr size, const void* data = nullptr, int usage = BufferImmutableUsage::DYNAMIC_STORAGE);
		extern void init_mutable(BufferTarget target, GLsizeiptr size, const void* data, BufferMutableUsage usage = BufferMutableUsage::DYNAMIC_DRAW);
		extern void subsend(BufferTarget target, GLintptr offset_bytes, GLsizeiptr size, const void* data);
		extern void map(BufferTarget target, void* data, size_t size);
		extern void submap(BufferTarget target, GLintptr offset_bytes, GLsizeiptr length_bytes, void* data);
		extern void copy_gl_buffer(ids::GLBuffer b_src, ids::GLBuffer b_dst, GLintptr offset_src_bytes, GLintptr offset_dst_bytes, GLsizeiptr size);
		extern void copy_bound_gl_buffers(GLintptr offset_src_bytes, GLintptr offset_dst_bytes, GLsizeiptr size);
		extern VoidArray read(BufferTarget target, GLintptr offset_bytes, GLsizeiptr size);
	}

	namespace draw
	{
		extern void arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count);
		extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, IndexDataType idt);
		extern void element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, IndexDataType idt);

		namespace instanced
		{
			extern void arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count);
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, IndexDataType idt);

			extern void arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count, GLuint first_instance);
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLuint first_instance, IndexDataType idt);
		}

		namespace base_vertex
		{
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt);
			extern void element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt);
		}

		namespace instanced_base_vertex
		{
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, IndexDataType idt);
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, GLuint first_instance, IndexDataType idt);
		}

		namespace multi
		{
			extern void arrays(DrawMode mode, const GLint* first_vertices, const GLsizei* vertex_counts, GLsizei drawcount);
			extern void elements(DrawMode mode, const GLsizei* index_counts, const GLsizeiptr* first_index_bytes, IndexDataType idt, GLsizei drawcount);
			extern void elements_base_vertex(DrawMode mode, GLsizei* index_counts, const GLintptr* first_index_bytes, IndexDataType idt, GLint* base_vertices, GLsizei drawcount);
		}
	}
}
