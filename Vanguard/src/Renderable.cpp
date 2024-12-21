#include "Renderable.h"

#include "Vanguard.h"

vg::VertexAttribute::VertexAttribute(ShaderAttribute attrib)
{
	switch (attrib.type)
	{
	case ShaderDataType::I:
	case ShaderDataType::I2:
	case ShaderDataType::I3:
	case ShaderDataType::I4:
		type = VertexAttributeDataType::INT;
		break;
	case ShaderDataType::UI:
	case ShaderDataType::UI2:
	case ShaderDataType::UI3:
	case ShaderDataType::UI4:
		type = VertexAttributeDataType::UINT;
		break;
	case ShaderDataType::F:
	case ShaderDataType::F2:
	case ShaderDataType::F3:
	case ShaderDataType::F4:
	case ShaderDataType::FM2:
	case ShaderDataType::FM2x3:
	case ShaderDataType::FM3x2:
	case ShaderDataType::FM2x4:
	case ShaderDataType::FM4x2:
	case ShaderDataType::FM3:
	case ShaderDataType::FM3x4:
	case ShaderDataType::FM4x3:
	case ShaderDataType::FM4:
		type = VertexAttributeDataType::FLOAT;
		break;
	case ShaderDataType::D:
	case ShaderDataType::D2:
	case ShaderDataType::D3:
	case ShaderDataType::D4:
	case ShaderDataType::DM2:
	case ShaderDataType::DM2x3:
	case ShaderDataType::DM3x2:
	case ShaderDataType::DM2x4:
	case ShaderDataType::DM4x2:
	case ShaderDataType::DM3:
	case ShaderDataType::DM3x4:
	case ShaderDataType::DM4x3:
	case ShaderDataType::DM4:
		type = VertexAttributeDataType::DOUBLE;
		break;
	}
	count = attrib.component_count();
}

// LATER investigate using glVertexAttribFormat+glBindVertexBuffer+glVertexAttribBinding workflow over glVertexAttribPointer

void vg::VertexAttribute::define_pointer(GLuint i, GLsizei stride) const
{
	if (type == VertexAttributeDataType::DOUBLE)
		glVertexAttribLPointer(i, count, (GLenum)type, stride, (void*)offset());
	else if (type != VertexAttributeDataType::FLOAT && type != VertexAttributeDataType::HALF && pass_by_integer)
		glVertexAttribIPointer(i, count, (GLenum)type, stride, (void*)offset());
	else
		glVertexAttribPointer(i, count, (GLenum)type, normalized, stride, (void*)offset());
	glEnableVertexAttribArray(i);
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(3, 3)
	glVertexAttribDivisor(i, instance_divisor);
#endif
}

GLsizei vg::VertexAttribute::offset() const
{
	GLuint type_offset = 0;
	switch (type)
	{
	case VertexAttributeDataType::CHAR:
		type_offset = sizeof(GLbyte);
		break;
	case VertexAttributeDataType::UCHAR:
		type_offset = sizeof(GLubyte);
		break;
	case VertexAttributeDataType::SHORT:
		type_offset = sizeof(GLshort);
		break;
	case VertexAttributeDataType::USHORT:
		type_offset = sizeof(GLushort);
		break;
	case VertexAttributeDataType::INT:
		type_offset = sizeof(GLint);
		break;
	case VertexAttributeDataType::UINT:
		type_offset = sizeof(GLuint);
		break;
	case VertexAttributeDataType::HALF:
		type_offset = sizeof(GLhalf);
		break;
	case VertexAttributeDataType::FLOAT:
		type_offset = sizeof(GLfloat);
		break;
	case VertexAttributeDataType::DOUBLE:
		type_offset = sizeof(GLdouble);
		break;
	case VertexAttributeDataType::FIXED:
		type_offset = sizeof(GLfixed);
		break;
	}
	return type_offset * count;
}

vg::CPUImmutableVertexBufferData::CPUImmutableVertexBufferData(const Shader& shader, GLuint vertex_count)
{
	buffers::init_immutable(BufferTarget::VERTEX, cpubuf.size());
	attributes.reserve(shader.layout().size());
	for (GLuint i = 0; i < shader.layout().size(); ++i)
	{
		VertexAttribute attrib(shader.layout()[i]);
		attributes.push_back(attrib);
		stride += attrib.offset();
	}
	for (GLuint i = 0; i < attributes.size(); ++i)
		attributes[i].define_pointer(i, stride);
	cpubuf.resize(stride * vertex_count);
}

vg::CPUImmutableVertexBufferData::CPUImmutableVertexBufferData(const Shader& shader, GLuint vertex_count, const VertexAttributeSpecificationList& specifications)
{
	buffers::init_immutable(BufferTarget::VERTEX, cpubuf.size());
	attributes.reserve(shader.layout().size());
	auto spec_iter = specifications.ordered_override_data_types.begin();
	for (GLuint i = 0; i < shader.layout().size(); ++i)
	{
		VertexAttribute attrib(shader.layout()[i]);
		if (spec_iter != specifications.ordered_override_data_types.end() && spec_iter->first == i)
		{
			attrib.type = spec_iter->second;
			++spec_iter;
		}
		attributes.push_back(attrib);
		stride += attrib.offset();
	}
	for (auto [i, normalized] : specifications.normalized)
		attributes[i].normalized = normalized;
	for (auto [i, instance_divisor] : specifications.instance_divisor)
		attributes[i].instance_divisor = instance_divisor;
	for (auto [i, pass_by_integer] : specifications.pass_by_integer)
		attributes[i].pass_by_integer = pass_by_integer;
	for (GLuint i = 0; i < attributes.size(); ++i)
		attributes[i].define_pointer(i, stride);
	cpubuf.resize(stride * vertex_count);
}
