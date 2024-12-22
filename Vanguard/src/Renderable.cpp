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

// LATER investigate using glVertexAttribFormat+glBindVertexBuffer+glVertexAttribBinding workflow over glVertexAttribPointer. Also investigate glBindVertexBuffer

void vg::VertexAttribute::attrib_pointer(GLuint i, GLsizei stride) const
{
#pragma warning(push)
#pragma warning(disable : 4312)
	if (type == VertexAttributeDataType::DOUBLE)
		glVertexAttribLPointer(i, count, (GLenum)type, stride, (void*)offset);
	else if (type != VertexAttributeDataType::FLOAT && type != VertexAttributeDataType::HALF && pass_by_integer)
		glVertexAttribIPointer(i, count, (GLenum)type, stride, (void*)offset);
	else
		glVertexAttribPointer(i, count, (GLenum)type, normalized, stride, (void*)offset); // TODO count must be 1, 2, 3, or 4??? How about matrices/arrays?
#pragma warning(pop)
	glEnableVertexAttribArray(i);
	glVertexAttribDivisor(i, instance_divisor);
}

GLsizei vg::VertexAttribute::bytes() const
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

vg::VertexBufferLayout::VertexBufferLayout(const Shader& shader)
{
	_attributes.reserve(shader.layout().size());
	for (GLuint i = 0; i < shader.layout().size(); ++i)
	{
		VertexAttribute attrib(shader.layout()[i]);
		attrib.offset = _stride;
		_stride += attrib.bytes();
		_attributes.push_back(attrib);
	}
}

vg::VertexBufferLayout::VertexBufferLayout(const Shader& shader, const VertexAttributeSpecificationList& specifications)
{
	_attributes.reserve(shader.layout().size());
	auto spec_iter = specifications.ordered_override_data_types.begin();
	for (GLuint i = 0; i < shader.layout().size(); ++i)
	{
		VertexAttribute attrib(shader.layout()[i]);
		if (spec_iter != specifications.ordered_override_data_types.end() && spec_iter->first == i)
		{
			attrib.type = spec_iter->second;
			++spec_iter;
		}
		attrib.offset = _stride;
		_stride += attrib.bytes();
		_attributes.push_back(attrib);
	}
	for (auto [i, normalized] : specifications.normalized)
		_attributes[i].normalized = normalized;
	for (auto [i, instance_divisor] : specifications.instance_divisor)
		_attributes[i].instance_divisor = instance_divisor;
	for (auto [i, pass_by_integer] : specifications.pass_by_integer)
		_attributes[i].pass_by_integer = pass_by_integer;
}

void vg::VertexBufferLayout::attrib_pointers() const
{
	for (GLuint i = 0; i < _attributes.size(); ++i)
		_attributes[i].attrib_pointer(i, _stride);
}

void vg::VertexBufferLayout::unattrib_pointers() const
{
	for (GLuint i = 0; i < _attributes.size(); ++i)
		glDisableVertexAttribArray(i);
}

GLintptr vg::VertexBufferLayout::buffer_offset(GLuint vertex, GLuint attrib) const
{
	return vertex * _stride + _attributes[attrib].offset;
}

vg::VAOBinding::VAOBinding(const Shader& shader)
	: _layout(shader)
{
}

vg::VAOBinding::VAOBinding(const Shader& shader, const VertexAttributeSpecificationList& specifications)
	: _layout(shader, specifications)
{
}

void vg::VAOBinding::attach_vertex_buffer(ids::GLBuffer vb) const
{
	va.bind();
	buffers::bind(vb, BufferTarget::VERTEX);
	_layout.attrib_pointers();
	unbind_vertex_array();
}

void vg::VAOBinding::attach_vertex_buffers(const std::initializer_list<ids::GLBuffer>& vbs)
{
	va.bind();
	for (ids::GLBuffer vb : vbs)
	{
		buffers::bind(vb, BufferTarget::VERTEX);
		_layout.attrib_pointers();
	}
	unbind_vertex_array();
}

void vg::VAOBinding::detach_vertex_buffer(ids::GLBuffer vb) const
{
	va.bind();
	buffers::bind(vb, BufferTarget::VERTEX);
	_layout.unattrib_pointers();
	unbind_vertex_array();
}

void vg::VAOBinding::detach_vertex_buffers(const std::initializer_list<ids::GLBuffer>& vbs)
{
	va.bind();
	for (ids::GLBuffer vb : vbs)
	{
		buffers::bind(vb, BufferTarget::VERTEX);
		_layout.unattrib_pointers();
	}
	unbind_vertex_array();
}

vg::VAOBindingBlock::VAOBindingBlock(const std::vector<Shader*>& shaders)
	: vas(shaders.size())
{
	_layouts.resize(vas.get_count());
	for (Shader* shader : shaders)
		_layouts.push_back(VertexBufferLayout(*shader));
}

vg::VAOBindingBlock::VAOBindingBlock(const std::vector<std::pair<Shader*, VertexAttributeSpecificationList*>>& shaders)
	: vas(shaders.size())
{
	_layouts.resize(vas.get_count());
	for (auto [shader, speclist] : shaders)
	{
		if (speclist)
			_layouts.push_back(VertexBufferLayout(*shader, *speclist));
		else
			_layouts.push_back(VertexBufferLayout(*shader));
	}
}

void vg::VAOBindingBlock::attach_vertex_buffer(GLuint i, ids::GLBuffer vb) const
{
	vas.bind(i);
	buffers::bind(vb, BufferTarget::VERTEX);
	_layouts[i].attrib_pointers();
	unbind_vertex_array();
}

void vg::VAOBindingBlock::attach_vertex_buffers(GLuint i, const std::initializer_list<ids::GLBuffer>& vbs)
{
	vas.bind(i);
	for (ids::GLBuffer vb : vbs)
	{
		buffers::bind(vb, BufferTarget::VERTEX);
		_layouts[i].attrib_pointers();
	}
	unbind_vertex_array();
}

void vg::VAOBindingBlock::attach_vertex_buffers(const std::vector<std::pair<GLuint, ids::GLBuffer>>& vbs)
{
	for (auto [i, vb] : vbs)
	{
		vas.bind(i);
		buffers::bind(vb, BufferTarget::VERTEX);
		_layouts[i].attrib_pointers();
	}
	unbind_vertex_array();
}

void vg::VAOBindingBlock::detach_vertex_buffer(GLuint i, ids::GLBuffer vb) const
{
	vas.bind(i);
	buffers::bind(vb, BufferTarget::VERTEX);
	_layouts[i].unattrib_pointers();
	unbind_vertex_array();
}

void vg::VAOBindingBlock::detach_vertex_buffers(GLuint i, const std::initializer_list<ids::GLBuffer>& vbs)
{
	vas.bind(i);
	for (ids::GLBuffer vb : vbs)
	{
		buffers::bind(vb, BufferTarget::VERTEX);
		_layouts[i].unattrib_pointers();
	}
	unbind_vertex_array();
}

void vg::VAOBindingBlock::detach_vertex_buffers(const std::vector<std::pair<GLuint, ids::GLBuffer>>& vbs)
{
	for (auto [i, vb] : vbs)
	{
		vas.bind(i);
		buffers::bind(vb, BufferTarget::VERTEX);
		_layouts[i].unattrib_pointers();
	}
	unbind_vertex_array();
}

void vg::buffers::init_immutable_cpu_vertex_buffer(VoidArray& cpubuf, const VertexBufferLayout& layout, GLuint vertex_count)
{
	cpubuf.resize(layout.stride() * vertex_count);
	buffers::init_immutable(BufferTarget::VERTEX, cpubuf.size());
}

vg::VoidArray vg::buffers::init_immutable_cpu_vertex_buffer(const VertexBuffer& vb, GLuint vertex_count)
{
	if (auto binding = vb.binding())
	{
		VoidArray v(binding->layout().stride() * vertex_count);
		vb.bind();
		buffers::init_immutable(BufferTarget::VERTEX, v.size());
		return v;
	}
	else
		return VoidArray();
}

void vg::buffers::init_mutable_cpu_vertex_buffer(VoidArray& cpubuf, const VertexBufferLayout& layout, GLuint vertex_count)
{
	cpubuf.resize(layout.stride() * vertex_count);
	buffers::init_mutable(BufferTarget::VERTEX, cpubuf.size(), cpubuf);
}

vg::VoidArray vg::buffers::init_mutable_cpu_vertex_buffer(const VertexBuffer& vb, GLuint vertex_count)
{
	if (auto binding = vb.binding())
	{
		VoidArray v(binding->layout().stride() * vertex_count);
		vb.bind();
		buffers::init_mutable(BufferTarget::VERTEX, v.size(), v);
		return v;
	}
	else
		return VoidArray();
}

vg::VertexBuffer::VertexBuffer(VAOBinding* binding)
{
	attach_to(binding);
}

void vg::VertexBuffer::attach_to(VAOBinding* binding)
{
	if (_binding != binding)
	{
		if (_binding)
			_binding->detach_vertex_buffer(_vb);
		_binding = binding;
		if (_binding)
			_binding->attach_vertex_buffer(_vb);
	}
}

void vg::VertexBuffer::bind() const
{
	buffers::bind(_vb, BufferTarget::VERTEX);
}

vg::VertexBufferBlock::VertexBufferBlock(GLuint block_count)
	: _vbs(block_count)
{
}

vg::VertexBufferBlock::VertexBufferBlock(GLuint block_count, VAOBinding* binding)
	: _vbs(block_count)
{
	attach_to(binding);
}

void vg::VertexBufferBlock::attach_to(VAOBinding* binding)
{
	if (_binding != binding)
	{
		if (_binding)
		{
			for (GLuint i = 0; i < _vbs.get_count(); ++i)
				_binding->detach_vertex_buffer(_vbs[i]);
		}
		_binding = binding;
		if (_binding)
		{
			for (GLuint i = 0; i < _vbs.get_count(); ++i)
				_binding->attach_vertex_buffer(_vbs[i]);
		}
	}
}

void vg::VertexBufferBlock::bind(GLuint i) const
{
	buffers::bind(_vbs[i], BufferTarget::VERTEX);
}
