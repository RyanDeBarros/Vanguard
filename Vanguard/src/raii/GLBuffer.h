#pragma once

#include "Vendor.h"
#include "utils/VoidArray.h"

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

			GLuint get_count() const { return count; }
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

		public:
			VertexArray();
			VertexArray(const VertexArray&) = delete;
			VertexArray(VertexArray&&) noexcept;
			VertexArray& operator=(VertexArray&&) noexcept;
			~VertexArray();

			operator ids::VertexArray () const { return _vao; }
			void bind() const;
		};

		class VertexArrayBlock
		{
			using V = ids::VertexArray;
			V* _vaos = nullptr;
			GLuint count;

		public:
			VertexArrayBlock(GLuint count);
			VertexArrayBlock(const VertexArrayBlock&) = delete;
			VertexArrayBlock(VertexArrayBlock&&) noexcept;
			VertexArrayBlock& operator=(VertexArrayBlock&&) noexcept;
			~VertexArrayBlock();

			GLuint get_count() const { return count; }

			operator const ids::VertexArray* () const { return _vaos; }
			ids::VertexArray operator[](GLuint i) const;
			void bind(GLuint i) const;
		};
	}

	extern void bind_index_buffer_to_vertex_array(ids::GLBuffer ib, ids::VertexArray va);
	extern void bind_index_buffers_to_vertex_arrays(const ids::GLBuffer* ibs, const ids::VertexArray* vas, GLuint count);
	extern void unbind_vertex_array();

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

		const GPUIndirectArrays& gpu_arrays() const { return g; }

		void bind() const { g.bind(); }

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

		GLuint get_count() const { return count; }

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

		const GPUIndirectElements& gpu_elements() const { return g; }

		void bind() const { g.bind(); }

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
		ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ = GL_COPY_READ_BUFFER,
		COPY_WRITE = GL_COPY_WRITE_BUFFER,
		DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
		DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
		INDEX = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_READ = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER,
		QUERY = GL_QUERY_BUFFER,
		SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
		TEXTURE = GL_TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
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
		extern void init_mutable(BufferTarget target, GLsizeiptr size, const void* data = nullptr, BufferMutableUsage usage = BufferMutableUsage::DYNAMIC_DRAW);
		extern void subsend(BufferTarget target, GLintptr offset_bytes, GLsizeiptr size, const void* data);
		extern void map(BufferTarget target, void* data, size_t size);
		extern void submap(BufferTarget target, GLintptr offset_bytes, GLsizeiptr length_bytes, void* data);
		extern void copy_gl_buffer(ids::GLBuffer b_src, ids::GLBuffer b_dst, GLintptr offset_src_bytes, GLintptr offset_dst_bytes, GLsizeiptr size);
		extern void copy_bound_gl_buffers(GLintptr offset_src_bytes, GLintptr offset_dst_bytes, GLsizeiptr size);
		extern VoidArray read(BufferTarget target, GLintptr offset_bytes, GLsizeiptr size);
		extern bool is_mutable(ids::GLBuffer buf);
		extern GLuint size(ids::GLBuffer buf);
	}
}
