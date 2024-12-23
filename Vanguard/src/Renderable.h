#pragma once

#include <array>

#include "raii/GLBuffer.h"
#include "raii/Shader.h"

namespace vg
{
	// TODO convert to char enum, but then define conversion function that will convert to OpenGL enums? Because GL_ enums don't fit in char.
	enum class VertexAttributeDataType
	{
		CHAR = GL_BYTE,
		UCHAR = GL_UNSIGNED_BYTE,
		SHORT = GL_SHORT,
		USHORT = GL_UNSIGNED_SHORT,
		INT = GL_INT,
		UINT = GL_UNSIGNED_INT,
		HALF = GL_HALF_FLOAT,
		FLOAT = GL_FLOAT,
		DOUBLE = GL_DOUBLE,
		FIXED = GL_FIXED
	};

	class VertexAttribute
	{
		VertexAttributeDataType type = VertexAttributeDataType::FLOAT;
		bool normalized = false;
		bool pass_by_integer = false;
		GLubyte rows = 0;
		GLuint instance_divisor = 0;
		GLuint offset = 0;
		GLushort location = 0;

	public:
		VertexAttribute(ShaderAttribute attrib, GLuint location, GLuint offset);
		
		void attrib_pointer(GLuint i, GLsizei stride) const;
		void attrib_pointer(GLuint i, GLsizei stride, GLuint offset) const;
		GLsizei bytes() const;
		void set_type(VertexAttributeDataType type) { this->type = type; }
		void set_normalized(bool normalized) { this->normalized = normalized; }
		void set_pass_by_integer(bool pass_by_integer) { this->pass_by_integer = pass_by_integer; }
		void set_instance_divisor(GLuint instance_divisor) { this->instance_divisor = instance_divisor; }
		GLuint get_offset() const { return offset; }

		static GLuint location_coverage(ShaderAttribute attrib);
	};

	struct VertexAttributeSpecificationList
	{
		std::vector<std::pair<GLuint, bool>> normalized = {};
		std::vector<std::pair<GLuint, bool>> pass_by_integer = {};
		std::vector<std::pair<GLuint, VertexAttributeDataType>> ordered_override_data_types = {};
		std::vector<std::pair<GLuint, GLuint>> instance_divisor = {};
	};

	class VertexBufferLayout
	{
		std::vector<VertexAttribute> _attributes; // TODO get_location_coverage() should just be 1. Instead, just use multiple attributes per covered location.
		GLuint _stride = 0;

	public:
		VertexBufferLayout(const Shader& shader);
		VertexBufferLayout(const Shader& shader, const VertexAttributeSpecificationList& specifications);

		GLuint stride() const { return _stride; }
		const std::vector<VertexAttribute>& attributes() const { return _attributes; }
		void attrib_pointer(GLuint attrib) const;
		void unattrib_pointer(GLuint attrib) const;

		GLintptr buffer_offset(GLuint vertex, GLuint attrib) const;
	};

	// Use VertexBuffer for sole attachments to a VAO. In other words, a VertexBuffer stores all attributes in a VertexBufferLayout.
	class VertexBuffer
	{
		std::shared_ptr<VertexBufferLayout> _layout;
		raii::VertexArray _vao;
		raii::GLBuffer _vb;

		void init() const;

	public:
		VertexBuffer(const std::shared_ptr<VertexBufferLayout>& layout);
		VertexBuffer(std::shared_ptr<VertexBufferLayout>&& layout);
		VertexBuffer(const VertexBuffer&) = delete;
		
		const std::shared_ptr<VertexBufferLayout>& layout() const { return _layout; }
		ids::VertexArray vao() const { return _vao; }
		ids::GLBuffer vb() const { return _vb; }
		void bind_vao() const;
		void bind_vb() const;

		GLintptr buffer_offset(GLuint vertex, GLuint attrib) const;

		template<typename Type>
		const Type& ref(const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.ref<Type>(buffer_offset(vertex, attrib));
		}

		template<typename Type>
		Type& ref(VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.ref<Type>(buffer_offset(vertex, attrib));
		}

		template<typename Type>
		Type val(const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.val<Type>(buffer_offset(vertex, attrib));
		}

		template<typename Type>
		void set_attribute(VoidArray& cpubuf, GLuint attrib, GLuint starting_vertex, GLuint count, const Type& obj)
		{
			for (GLuint i = 0; i < count; ++i)
				cpubuf.ref<Type>(buffer_offset(starting_vertex + i, attrib)) = obj;
		}

		template<typename Type>
		void set_attribute(VoidArray& cpubuf, GLuint attrib, const Type& obj)
		{
			GLuint vcount = vertex_count(cpubuf);
			for (GLuint i = 0; i < vcount; ++i)
				cpubuf.ref<Type>(buffer_offset(i, attrib)) = obj;
		}

		template<typename Type, size_t N>
		void set_attributes(VoidArray& cpubuf, GLuint attrib, GLuint starting_vertex, const std::array<Type, N>& objs)
		{
			for (GLuint i = 0; i < N; ++i)
				cpubuf.ref<Type>(buffer_offset(starting_vertex + i, attrib)) = objs[i];
		}

		VoidArray init_immutable_cpu_buffer(GLuint vertex_count) const;
		VoidArray init_mutable_cpu_buffer(GLuint vertex_count) const;
		GLuint vertex_count(const VoidArray& cpubuf) const { return GLuint(cpubuf.size() / _layout->stride()); }
	};

	// Use VertexBufferBlock for separated attachments to a VAO. In other words, a VertexBufferBlock's individual buffers each store different attributes in a VertexBufferLayout.
	class VertexBufferBlock
	{
		std::shared_ptr<VertexBufferLayout> _layout;
		std::vector<GLuint> _strides;
		std::vector<GLuint> _offsets;
		raii::VertexArray _vao;
		raii::GLBufferBlock _vbs;

		void init(const std::initializer_list<std::pair<GLuint, std::initializer_list<GLuint>>>& attributes);

	public:
		VertexBufferBlock(GLuint block_count, const std::shared_ptr<VertexBufferLayout>& layout, const std::initializer_list<std::pair<GLuint, std::initializer_list<GLuint>>>& attributes);
		VertexBufferBlock(GLuint block_count, std::shared_ptr<VertexBufferLayout>&& layout, const std::initializer_list<std::pair<GLuint, std::initializer_list<GLuint>>>& attributes);
		VertexBufferBlock(const VertexBufferBlock&) = delete;

		const std::shared_ptr<VertexBufferLayout>& layout() const { return _layout; }
		ids::VertexArray vao() const { return _vao; }
		ids::GLBuffer vb(GLuint i) const { return _vbs[i]; }
		GLuint vb_stride(GLuint i) const { return _strides[i]; }
		void bind_vb(GLuint i) const;
		void bind_vao() const;
		GLuint block_count() const { return _vbs.get_count(); }
		GLintptr buffer_offset(GLuint i, GLuint vertex, GLuint attrib) const;

		template<typename Type>
		const Type& ref(const VoidArray& cpubuf, GLuint i, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.ref<Type>(buffer_offset(i, vertex, attrib));
		}

		template<typename Type>
		Type& ref(VoidArray& cpubuf, GLuint i, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.ref<Type>(buffer_offset(i, vertex, attrib));
		}

		template<typename Type>
		Type val(const VoidArray& cpubuf, GLuint i, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.val<Type>(buffer_offset(i, vertex, attrib));
		}

		template<typename Type>
		void set_attribute(VoidArray& cpubuf, GLuint i, GLuint attrib, GLuint starting_vertex, GLuint count, const Type& obj)
		{
			for (GLuint v = 0; v < count; ++v)
				cpubuf.ref<Type>(buffer_offset(i, starting_vertex + v, attrib)) = obj;
		}

		template<typename Type>
		void set_attribute(VoidArray& cpubuf, GLuint i, GLuint attrib, const Type& obj)
		{
			GLuint vcount = vertex_count(i, cpubuf);
			for (GLuint v = 0; v < vcount; ++v)
				cpubuf.ref<Type>(buffer_offset(i, v, attrib)) = obj;
		}

		template<typename Type, size_t N>
		void set_attributes(VoidArray& cpubuf, GLuint i, GLuint attrib, GLuint starting_vertex, const std::array<Type, N>& objs)
		{
			for (GLuint v = 0; v < N; ++v)
				cpubuf.ref<Type>(buffer_offset(i, starting_vertex + v, attrib)) = objs[v];
		}

		VoidArray init_immutable_cpu_buffer(GLuint i, GLuint vertex_count) const;
		VoidArray init_mutable_cpu_buffer(GLuint i, GLuint vertex_count) const;
		GLuint vertex_count(GLuint i, const VoidArray& cpubuf) const { return GLuint(cpubuf.size() / vb_stride(i)); }
	};

	// Use MultiVertexBuffer for a group of sole attachments to a group of VAOs. In other words, a MultiVertexBuffer is just a fixed-size vector of VertexBuffer-VertexArray pairs that are stored optimally in GPU memory.
	class MultiVertexBuffer
	{
		std::vector<std::shared_ptr<VertexBufferLayout>> _layouts;
		raii::VertexArrayBlock _vaos;
		raii::GLBufferBlock _vbs;

		void init() const;

	public:
		MultiVertexBuffer(const std::vector<std::shared_ptr<VertexBufferLayout>>& layouts);
		MultiVertexBuffer(std::vector<std::shared_ptr<VertexBufferLayout>>&& layouts);
		MultiVertexBuffer(const MultiVertexBuffer&) = delete;

		// TODO in all Block classes, use a struct BlockIndex instead of GLuint

		const std::shared_ptr<VertexBufferLayout>& layout(GLuint i) const { return _layouts[i]; }
		ids::VertexArray vao(GLuint i) const { return _vaos[i]; }
		ids::GLBuffer vb(GLuint i) const { return _vbs[i]; }
		void bind_vao(GLuint i) const;
		void bind_vb(GLuint i) const;
		GLuint block_count() const { return (GLuint)_layouts.size(); }

		GLintptr buffer_offset(GLuint i, GLuint vertex, GLuint attrib) const;

		template<typename Type>
		const Type& ref(GLuint i, const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.ref<Type>(buffer_offset(i, vertex, attrib));
		}

		template<typename Type>
		Type& ref(GLuint i, VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.ref<Type>(buffer_offset(i, vertex, attrib));
		}

		template<typename Type>
		Type val(GLuint i, const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			return cpubuf.val<Type>(buffer_offset(i, vertex, attrib));
		}

		template<typename Type>
		void set_attribute(GLuint i, VoidArray& cpubuf, GLuint attrib, GLuint starting_vertex, GLuint count, const Type& obj)
		{
			for (GLuint v = 0; v < count; ++v)
				cpubuf.ref<Type>(buffer_offset(i, starting_vertex + v, attrib)) = obj;
		}

		template<typename Type>
		void set_attribute(GLuint i, VoidArray& cpubuf, GLuint attrib, const Type& obj)
		{
			GLuint vcount = vertex_count(i, cpubuf);
			for (GLuint v = 0; v < vcount; ++v)
				cpubuf.ref<Type>(buffer_offset(i, v, attrib)) = obj;
		}

		template<typename Type, size_t N>
		void set_attributes(GLuint i, VoidArray& cpubuf, GLuint attrib, GLuint starting_vertex, const std::array<Type, N>& objs)
		{
			for (GLuint v = 0; v < N; ++v)
				cpubuf.ref<Type>(buffer_offset(i, starting_vertex + v, attrib)) = objs[v];
		}

		VoidArray init_immutable_cpu_buffer(GLuint i, GLuint vertex_count) const;
		VoidArray init_mutable_cpu_buffer(GLuint i, GLuint vertex_count) const;
		GLuint vertex_count(GLuint i, const VoidArray& cpubuf) const { return GLuint(cpubuf.size() / _layouts[i]->stride()); }
	};
}
