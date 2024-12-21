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

	struct VertexAttribute
	{
		VertexAttributeDataType type = VertexAttributeDataType::FLOAT;
		GLsizei count = 0;
		bool normalized = false;
		bool pass_by_integer = false;
		GLuint instance_divisor = 0;

		VertexAttribute(ShaderAttribute attrib);
		
		void define_pointer(GLuint i, GLsizei stride) const;
		GLsizei offset() const;
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
		void define_pointers() const;
	};

	class VAOBinding
	{
		VertexBufferLayout _layout;
		raii::VertexArray va;

	public:
		VAOBinding(const Shader& shader);
		VAOBinding(const Shader& shader, const VertexAttributeSpecificationList& specifications);

		const VertexBufferLayout& layout() const { return _layout; }
		const raii::VertexArray& vertex_array() const { return va; }
		void attach_vertex_buffer(ids::GLBuffer vb) const;
		void attach_vertex_buffers(const std::initializer_list<ids::GLBuffer>& vbs);
	};

	// TODO VAOBlockBinding. Also, define a VertexBuffer class that has a VAOBinding and a fixed size number of vertex buffers, and use a block for that.

	namespace buffers
	{
		extern void init_immutable_cpu_vertex_buffer(VoidArray& cpubuf, const VertexBufferLayout& layout, GLuint vertex_count);
	}

	// TODO CPUMutableVertexBufferDatas

	// TODO float-only immutable/mutable vertex buffer for simplicity and performance in that specific use-case

	// TODO GPU versions for write-only
}
