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

	class CPUImmutableVertexBufferData
	{
		VoidArray cpubuf;
		std::vector<VertexAttribute> attributes;
		GLuint stride = 0;

	public:
		CPUImmutableVertexBufferData(const Shader& shader, GLuint vertex_count);
		CPUImmutableVertexBufferData(const Shader& shader, GLuint vertex_count, const VertexAttributeSpecificationList& specifications);
	};

	// TODO CPUMutableVertexBufferDatas

	// TODO float-only immutable/mutable vertex buffer for simplicity and performance in that specific use-case

	// TODO GPU versions for write-only
}
