#pragma once

#include "raii/GLBuffer.h"
#include "raii/Shader.h"

namespace vg
{
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
		GLubyte rows;
		GLuint instance_divisor = 0;
		GLuint offset = 0;
		GLushort location = 0;
		GLushort location_coverage = 0;

	public:
		VertexAttribute(ShaderAttribute attrib, GLuint location, GLuint offset);
		
		void attrib_pointer(GLuint i, GLsizei stride) const;
		GLsizei bytes() const;
		GLuint get_location_coverage() const { return location_coverage; }
		void set_type(VertexAttributeDataType type) { this->type = type; }
		void set_normalized(bool normalized) { this->normalized = normalized; }
		void set_pass_by_integer(bool pass_by_integer) { this->pass_by_integer = pass_by_integer; }
		void set_instance_divisor(GLuint instance_divisor) { this->instance_divisor = instance_divisor; }
		GLuint get_offset() const { return offset; }
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
		std::vector<VertexAttribute> _attributes;
		GLuint _stride = 0;

	public:
		VertexBufferLayout(const Shader& shader);
		VertexBufferLayout(const Shader& shader, const VertexAttributeSpecificationList& specifications);

		GLuint stride() const { return _stride; }
		const std::vector<VertexAttribute>& attributes() const { return _attributes; }
		void attrib_pointers() const;
		void unattrib_pointers() const;

		GLintptr buffer_offset(GLuint vertex, GLuint attrib) const;
	};

	class VAOBinding
	{
		VertexBufferLayout _layout;
		raii::VertexArray va;

	public:
		VAOBinding(const Shader& shader);
		VAOBinding(const Shader& shader, const VertexAttributeSpecificationList& specifications);

		const VertexBufferLayout& layout() const { return _layout; }
		ids::VertexArray vertex_array() const { return va; }
		void bind() const { va.bind(); }
		void bind_index_buffer(ids::GLBuffer ib) const { bind_index_buffer_to_vertex_array(ib, va); }
		void attach_vertex_buffer(ids::GLBuffer vb) const;
		void attach_vertex_buffers(const std::initializer_list<ids::GLBuffer>& vbs);
		void detach_vertex_buffer(ids::GLBuffer vb) const;
		void detach_vertex_buffers(const std::initializer_list<ids::GLBuffer>& vbs);
	};

	class VAOBindingBlock
	{
		std::vector<VertexBufferLayout> _layouts; // TODO define fixed_vector for dynamic array that doesn't need push_back/erase functions
		raii::VertexArrayBlock vas;

	public:
		VAOBindingBlock(const std::vector<Shader*>& shaders);
		VAOBindingBlock(const std::vector<std::pair<Shader*, VertexAttributeSpecificationList*>>& shaders);

		const VertexBufferLayout& layout(GLuint i) const { return _layouts[i]; }
		ids::VertexArray vertex_array(GLuint i) const { return vas[i]; }
		void bind(GLuint i) const { vas.bind(i); }
		void bind_index_buffer(GLuint i, ids::GLBuffer ib) const { bind_index_buffer_to_vertex_array(ib, vas[i]); }
		void bind_index_buffers(GLuint starting_i, const ids::GLBuffer* ibs, GLuint count) const { bind_index_buffers_to_vertex_arrays(ibs, vas + starting_i, count); }
		void attach_vertex_buffer(GLuint i, ids::GLBuffer vb) const;
		void attach_vertex_buffers(GLuint i, const std::initializer_list<ids::GLBuffer>& vbs);
		void attach_vertex_buffers(const std::vector<std::pair<GLuint, ids::GLBuffer>>& vbs);
		void detach_vertex_buffer(GLuint i, ids::GLBuffer vb) const;
		void detach_vertex_buffers(GLuint i, const std::initializer_list<ids::GLBuffer>& vbs);
		void detach_vertex_buffers(const std::vector<std::pair<GLuint, ids::GLBuffer>>& vbs);
	};

	class VertexBuffer
	{
		VAOBinding* _binding = nullptr; // TODO mechanism to allow for attaching to vertex array in VAOBindingBlock. So instead, use pointer to VertexBufferLayout and just store the ids::VertexArray
		raii::GLBuffer _vb;

	public:
		VertexBuffer() = default;
		VertexBuffer(VAOBinding* binding);

		VertexBuffer(const VertexBuffer&) = delete; // TODO
		VertexBuffer(VertexBuffer&&) = delete; // TODO

		void attach_to(VAOBinding* binding);
		const VAOBinding* binding() const { return _binding; }
		VAOBinding* binding() { return _binding; }
		const raii::GLBuffer& vb() const { return _vb; }
		void bind() const;

		template<typename Type>
		const Type& ref(const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			if (!_binding)
				throw Error(ErrorCode::NULL_POINTER);
			return cpubuf.ref<Type>(_binding->layout().buffer_offset(vertex, attrib));
		}

		template<typename Type>
		Type& ref(VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			if (!_binding)
				throw Error(ErrorCode::NULL_POINTER);
			return cpubuf.ref<Type>(_binding->layout().buffer_offset(vertex, attrib));
		}

		template<typename Type>
		Type val(const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			if (!_binding)
				throw Error(ErrorCode::NULL_POINTER);
			return cpubuf.val<Type>(_binding->layout().buffer_offset(vertex, attrib));
		}
	};

	class VertexBufferBlock
	{
		VAOBinding* _binding = nullptr;
		raii::GLBufferBlock _vbs;

	public:
		VertexBufferBlock(GLuint block_count);
		VertexBufferBlock(GLuint block_count, VAOBinding* binding);

		VertexBufferBlock(const VertexBuffer&) = delete; // TODO
		VertexBufferBlock(VertexBuffer&&) = delete; // TODO

		void attach_to(VAOBinding* binding);
		const VAOBinding* binding() const { return _binding; }
		VAOBinding* binding() { return _binding; }
		const raii::GLBufferBlock& vbs() const { return _vbs; }
		void bind(GLuint i) const;
		GLuint block_count() const { return _vbs.get_count(); }

		template<typename Type>
		const Type& ref(const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			if (!_binding)
				throw Error(ErrorCode::NULL_POINTER);
			return cpubuf.ref<Type>(_binding->layout().buffer_offset(vertex, attrib));
		}

		template<typename Type>
		Type& ref(VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			if (!_binding)
				throw Error(ErrorCode::NULL_POINTER);
			return cpubuf.ref<Type>(_binding->layout().buffer_offset(vertex, attrib));
		}

		template<typename Type>
		Type val(const VoidArray& cpubuf, GLuint vertex, GLuint attrib) const
		{
			if (!_binding)
				throw Error(ErrorCode::NULL_POINTER);
			return cpubuf.val<Type>(_binding->layout().buffer_offset(vertex, attrib));
		}
	};

	namespace buffers
	{
		extern void init_immutable_cpu_vertex_buffer(VoidArray& cpubuf, const VertexBufferLayout& layout, GLuint vertex_count);
		extern VoidArray init_immutable_cpu_vertex_buffer(const VertexBuffer& vb, GLuint vertex_count);
		extern void init_mutable_cpu_vertex_buffer(VoidArray& cpubuf, const VertexBufferLayout& layout, GLuint vertex_count);
		extern VoidArray init_mutable_cpu_vertex_buffer(const VertexBuffer& vb, GLuint vertex_count);
	}
}
