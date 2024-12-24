#include "Renderable.h"

#include "Vanguard.h"

vg::VertexAttribute::VertexAttribute(ShaderAttribute attrib, GLuint location, GLuint offset)
	: location(location), offset(offset)
{
	switch (attrib.type)
	{
	case ShaderDataType::I:
	case ShaderDataType::I2:
	case ShaderDataType::I3:
	case ShaderDataType::I4:
		type = DataType::INT;
		break;
	case ShaderDataType::UI:
	case ShaderDataType::UI2:
	case ShaderDataType::UI3:
	case ShaderDataType::UI4:
		type = DataType::UINT;
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
		type = DataType::FLOAT;
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
		type = DataType::DOUBLE;
		break;
	}
	switch (attrib.type)
	{
	case ShaderDataType::I:
	case ShaderDataType::UI:
	case ShaderDataType::F:
	case ShaderDataType::D:
		rows = 1;
		break;
	case ShaderDataType::I2:
	case ShaderDataType::UI2:
	case ShaderDataType::F2:
	case ShaderDataType::D2:
	case ShaderDataType::FM2:
	case ShaderDataType::FM2x3:
	case ShaderDataType::FM2x4:
	case ShaderDataType::DM2:
	case ShaderDataType::DM2x3:
	case ShaderDataType::DM2x4:
		rows = 2;
		break;
	case ShaderDataType::I3:
	case ShaderDataType::UI3:
	case ShaderDataType::F3:
	case ShaderDataType::D3:
	case ShaderDataType::FM3x2:
	case ShaderDataType::FM3:
	case ShaderDataType::FM3x4:
	case ShaderDataType::DM3x2:
	case ShaderDataType::DM3:
	case ShaderDataType::DM3x4:
		rows = 3;
		break;
	case ShaderDataType::I4:
	case ShaderDataType::UI4:
	case ShaderDataType::F4:
	case ShaderDataType::D4:
	case ShaderDataType::FM4x2:
	case ShaderDataType::FM4x3:
	case ShaderDataType::FM4:
	case ShaderDataType::DM4x2:
	case ShaderDataType::DM4x3:
	case ShaderDataType::DM4:
		rows = 4;
		break;
	}
}

// LATER investigate using glVertexAttribFormat+glBindVertexBuffer+glVertexAttribBinding workflow over glVertexAttribPointer. Also investigate glBindVertexBuffer

void vg::VertexAttribute::attrib_pointer(GLuint i, GLsizei stride) const
{
#pragma warning(push)
#pragma warning(disable : 4312)
	if (type == DataType::DOUBLE)
		glVertexAttribLPointer(i, rows, type_as_gl_enum(), stride, (void*)offset);
	else if (type != DataType::FLOAT && type != DataType::HALF && pass_by_integer)
		glVertexAttribIPointer(i, rows, type_as_gl_enum(), stride, (void*)offset);
	else
		glVertexAttribPointer(i, rows, type_as_gl_enum(), normalized, stride, (void*)offset);
#pragma warning(pop)
	glEnableVertexAttribArray(i);
	glVertexAttribDivisor(i, instance_divisor);
}

void vg::VertexAttribute::attrib_pointer(GLuint i, GLsizei stride, GLuint offset) const
{
#pragma warning(push)
#pragma warning(disable : 4312)
	if (type == DataType::DOUBLE)
		glVertexAttribLPointer(i, rows, type_as_gl_enum(), stride, (void*)offset);
	else if (type != DataType::FLOAT && type != DataType::HALF && pass_by_integer)
		glVertexAttribIPointer(i, rows, type_as_gl_enum(), stride, (void*)offset);
	else
		glVertexAttribPointer(i, rows, type_as_gl_enum(), normalized, stride, (void*)offset);
#pragma warning(pop)
	glEnableVertexAttribArray(i);
	glVertexAttribDivisor(i, instance_divisor);
}

GLsizei vg::VertexAttribute::bytes() const
{
	GLuint type_offset = 0;
	switch (type)
	{
	case DataType::CHAR:
		type_offset = sizeof(GLbyte);
		break;
	case DataType::UCHAR:
		type_offset = sizeof(GLubyte);
		break;
	case DataType::SHORT:
		type_offset = sizeof(GLshort);
		break;
	case DataType::USHORT:
		type_offset = sizeof(GLushort);
		break;
	case DataType::INT:
		type_offset = sizeof(GLint);
		break;
	case DataType::UINT:
		type_offset = sizeof(GLuint);
		break;
	case DataType::HALF:
		type_offset = sizeof(GLhalf);
		break;
	case DataType::FLOAT:
		type_offset = sizeof(GLfloat);
		break;
	case DataType::DOUBLE:
		type_offset = sizeof(GLdouble);
		break;
	case DataType::FIXED:
		type_offset = sizeof(GLfixed);
		break;
	}
	return type_offset * rows;
}

GLuint vg::VertexAttribute::location_coverage(ShaderAttribute attrib)
{
	GLuint columns = 1;
	switch (attrib.type)
	{
	case ShaderDataType::FM2:
	case ShaderDataType::FM3x2:
	case ShaderDataType::FM4x2:
	case ShaderDataType::DM2:
	case ShaderDataType::DM3x2:
	case ShaderDataType::DM4x2:
		columns = 2;
		break;
	case ShaderDataType::FM2x3:
	case ShaderDataType::FM3:
	case ShaderDataType::FM4x3:
	case ShaderDataType::DM2x3:
	case ShaderDataType::DM3:
	case ShaderDataType::DM4x3:
		columns = 3;
		break;
	case ShaderDataType::FM2x4:
	case ShaderDataType::FM3x4:
	case ShaderDataType::FM4:
	case ShaderDataType::DM2x4:
	case ShaderDataType::DM3x4:
	case ShaderDataType::DM4:
		columns = 4;
		break;
	}
	return columns * attrib.array_count;
}

vg::VertexBufferLayout::VertexBufferLayout(const Shader& shader)
{
	_attributes.reserve(shader.layout().size());
	GLuint location = 0;
	for (GLuint i = 0; i < shader.layout().size(); ++i)
	{
		ShaderAttribute satt = shader.layout()[i];
		GLuint coverage = VertexAttribute::location_coverage(satt);
		for (GLuint i = 0; i < coverage; ++i)
		{
			VertexAttribute attrib(satt, location++, _stride);
			_stride += attrib.bytes();
			_attributes.push_back(attrib);
		}
	}
}

vg::VertexBufferLayout::VertexBufferLayout(const Shader& shader, const VertexAttributeSpecificationList& specifications)
{
	_attributes.reserve(shader.layout().size());
	auto spec_iter = specifications.ordered_override_data_types.begin();
	GLuint location = 0;
	for (GLuint i = 0; i < shader.layout().size(); ++i)
	{
		ShaderAttribute satt = shader.layout()[i];
		GLuint coverage = VertexAttribute::location_coverage(satt);
		for (GLuint i = 0; i < coverage; ++i)
		{
			VertexAttribute attrib(satt, location++, _stride);
			if (spec_iter != specifications.ordered_override_data_types.end() && spec_iter->first == i)
			{
				attrib.set_type(spec_iter->second);
				++spec_iter;
			}
			_stride += attrib.bytes();
			_attributes.push_back(attrib);
		}
	}
	for (auto [i, normalized] : specifications.normalized)
		_attributes[i].set_normalized(normalized);
	for (auto [i, instance_divisor] : specifications.instance_divisor)
		_attributes[i].set_instance_divisor(instance_divisor);
	for (auto [i, pass_by_integer] : specifications.pass_by_integer)
		_attributes[i].set_pass_by_integer(pass_by_integer);
}

void vg::VertexBufferLayout::attrib_pointer(GLuint attrib) const
{
	_attributes[attrib].attrib_pointer(attrib, _stride);
}

void vg::VertexBufferLayout::unattrib_pointer(GLuint attrib) const
{
	glDisableVertexAttribArray(attrib);
}

GLintptr vg::VertexBufferLayout::buffer_offset(GLuint vertex, GLuint attrib) const
{
	return vertex * _stride + _attributes[attrib].get_offset();
}

void vg::VertexBuffer::init() const
{
	bind_vao();
	bind_vb();
	for (GLuint i = 0; i < _layout->attributes().size(); ++i)
		_layout->attrib_pointer(i);
	unbind_vertex_array();
}

vg::VertexBuffer::VertexBuffer(const std::shared_ptr<VertexBufferLayout>& layout)
	: _layout(layout)
{
	init();
}

vg::VertexBuffer::VertexBuffer(std::shared_ptr<VertexBufferLayout>&& layout)
	: _layout(std::move(layout))
{
	init();
}

void vg::VertexBuffer::bind_vao() const
{
	_vao.bind();
}

void vg::VertexBuffer::bind_vb() const
{
	buffers::bind(_vb, BufferTarget::VERTEX);
}

GLintptr vg::VertexBuffer::buffer_offset(GLuint vertex, GLuint attrib) const
{
	return vertex * _layout->stride() + _layout->attributes()[attrib].get_offset();
}

vg::VoidArray vg::VertexBuffer::init_immutable_cpu_buffer(GLuint vertex_count) const
{
	VoidArray v(_layout->stride() * vertex_count);
	bind_vb();
	buffers::init_immutable(BufferTarget::VERTEX, v.size());
	return v;
}

vg::VoidArray vg::VertexBuffer::init_mutable_cpu_buffer(GLuint vertex_count) const
{
	VoidArray v(_layout->stride() * vertex_count);
	bind_vb();
	buffers::init_mutable(BufferTarget::VERTEX, v.size());
	return v;
}

void vg::VertexBufferBlock::init(const std::initializer_list<std::pair<GLuint, std::initializer_list<GLuint>>>& attributes)
{
	bind_vao();
	_offsets.resize(_layout->attributes().size());
	_strides.resize(_vbs.get_count());
	for (const auto& subattribs : attributes)
	{
		if (subattribs.first < _vbs.get_count())
		{
			bind_vb(subattribs.first);
			const auto& lattrs = _layout->attributes();
			GLuint stride = 0;
			for (GLuint attrib : subattribs.second)
				stride += lattrs[attrib].bytes();
			_strides[subattribs.first] = stride;

			GLuint offset = 0;
			for (GLuint attrib : subattribs.second)
			{
				lattrs[attrib].attrib_pointer(attrib, stride, offset);
				_offsets[attrib] = offset;
				offset += lattrs[attrib].bytes();
			}
		}
	}
	unbind_vertex_array();
}

vg::VertexBufferBlock::VertexBufferBlock(GLuint block_count, const std::shared_ptr<VertexBufferLayout>& layout,
	const std::initializer_list<std::pair<GLuint, std::initializer_list<GLuint>>>& attributes)
	: _layout(layout), _vbs(block_count)
{
	init(attributes);
}

vg::VertexBufferBlock::VertexBufferBlock(GLuint block_count, std::shared_ptr<VertexBufferLayout>&& layout,
	const std::initializer_list<std::pair<GLuint, std::initializer_list<GLuint>>>& attributes)
	: _layout(std::move(layout)), _vbs(block_count)
{
	init(attributes);
}

void vg::VertexBufferBlock::bind_vb(GLuint i) const
{
	buffers::bind(_vbs[i], BufferTarget::VERTEX);
}

void vg::VertexBufferBlock::bind_vao() const
{
	_vao.bind();
}

GLintptr vg::VertexBufferBlock::buffer_offset(GLuint i, GLuint vertex, GLuint attrib) const
{
	return vertex * vb_stride(i) + _offsets[attrib];
}

vg::VoidArray vg::VertexBufferBlock::init_immutable_cpu_buffer(GLuint i, GLuint vertex_count) const
{
	VoidArray v(vb_stride(i) * vertex_count);
	bind_vb(i);
	buffers::init_immutable(BufferTarget::VERTEX, v.size());
	return v;
}

vg::VoidArray vg::VertexBufferBlock::init_mutable_cpu_buffer(GLuint i, GLuint vertex_count) const
{
	VoidArray v(vb_stride(i) * vertex_count);
	bind_vb(i);
	buffers::init_mutable(BufferTarget::VERTEX, v.size());
	return v;
}

void vg::MultiVertexBuffer::init() const
{
	for (GLuint i = 0; i < _layouts.size(); ++i)
	{
		bind_vao(i);
		bind_vb(i);
		GLuint end = (GLuint)_layouts[i]->attributes().size();
		for (GLuint attrib = 0; attrib < end; ++attrib)
			_layouts[i]->attrib_pointer(attrib);
		unbind_vertex_array();
	}
}

vg::MultiVertexBuffer::MultiVertexBuffer(const std::vector<std::shared_ptr<VertexBufferLayout>>& layouts)
	: _layouts(layouts), _vaos((GLuint)layouts.size()), _vbs((GLuint)layouts.size())
{
	init();
}

vg::MultiVertexBuffer::MultiVertexBuffer(std::vector<std::shared_ptr<VertexBufferLayout>>&& layouts)
	: _layouts(std::move(layouts)), _vaos((GLuint)_layouts.size()), _vbs((GLuint)_layouts.size())
{
	init();
}

void vg::MultiVertexBuffer::bind_vao(GLuint i) const
{
	_vaos.bind(i);
}

void vg::MultiVertexBuffer::bind_vb(GLuint i) const
{
	buffers::bind(_vbs[i], BufferTarget::VERTEX);
}

GLintptr vg::MultiVertexBuffer::buffer_offset(GLuint i, GLuint vertex, GLuint attrib) const
{
	return vertex * _layouts[i]->stride() + _layouts[i]->attributes()[attrib].get_offset();
}

vg::VoidArray vg::MultiVertexBuffer::init_immutable_cpu_buffer(GLuint i, GLuint vertex_count) const
{
	VoidArray v(_layouts[i]->stride() * vertex_count);
	bind_vb(i);
	buffers::init_immutable(BufferTarget::VERTEX, v.size());
	return v;
}

vg::VoidArray vg::MultiVertexBuffer::init_mutable_cpu_buffer(GLuint i, GLuint vertex_count) const
{
	VoidArray v(_layouts[i]->stride() * vertex_count);
	bind_vb(i);
	buffers::init_mutable(BufferTarget::VERTEX, v.size());
	return v;
}

template<typename DataType>
static void fill_quad_indexes(vg::VoidArray& cpubuf, GLuint num_quads)
{
	for (GLuint i = 0; i < num_quads; ++i)
	{
		cpubuf.ref<DataType>((0 + 6 * i) * sizeof(DataType)) = 0 + 4 * i;
		cpubuf.ref<DataType>((1 + 6 * i) * sizeof(DataType)) = 1 + 4 * i;
		cpubuf.ref<DataType>((2 + 6 * i) * sizeof(DataType)) = 2 + 4 * i;
		cpubuf.ref<DataType>((3 + 6 * i) * sizeof(DataType)) = 2 + 4 * i;
		cpubuf.ref<DataType>((4 + 6 * i) * sizeof(DataType)) = 3 + 4 * i;
		cpubuf.ref<DataType>((5 + 6 * i) * sizeof(DataType)) = 0 + 4 * i;
	}
}

void vg::CPUIndexBuffer::init_immutable_quads(GLuint num_quads)
{
	cpubuf.resize(num_quads * 6 * index_data_type_size(idt));

	if (idt == IndexDataType::UBYTE)
		fill_quad_indexes<GLubyte>(cpubuf, num_quads);
	else if (idt == IndexDataType::USHORT)
		fill_quad_indexes<GLushort>(cpubuf, num_quads);
	else if (idt == IndexDataType::UINT)
		fill_quad_indexes<GLuint>(cpubuf, num_quads);

	init_immutable();
}

void vg::CPUIndexBuffer::init_mutable_quads(GLuint num_quads)
{
	cpubuf.resize(num_quads * 6 * index_data_type_size(idt));

	if (idt == IndexDataType::UBYTE)
		fill_quad_indexes<GLubyte>(cpubuf, num_quads);
	else if (idt == IndexDataType::USHORT)
		fill_quad_indexes<GLushort>(cpubuf, num_quads);
	else if (idt == IndexDataType::UINT)
		fill_quad_indexes<GLuint>(cpubuf, num_quads);

	init_mutable();
}

vg::CPUIndexBufferBlock::CPUIndexBufferBlock(const std::vector<IndexDataType>& idts)
	: _ibs((GLuint)idts.size())
{
	idt_cpubufs.reserve(_ibs.get_count());
	for (IndexDataType idt : idts)
		idt_cpubufs.push_back({ idt, VoidArray() });
}

void vg::CPUIndexBufferBlock::init_immutable(GLuint i, GLsizei count)
{
	auto& idt_cpubuf = idt_cpubufs[i];
	idt_cpubuf.second.resize(count * index_data_type_size(idt_cpubuf.first));
	bind(i);
	buffers::init_immutable(BufferTarget::INDEX, idt_cpubuf.second.size(), idt_cpubuf.second);
}

void vg::CPUIndexBufferBlock::init_immutable(GLuint i)
{
	bind(i);
	const auto& cpubuf = idt_cpubufs[i].second;
	buffers::init_immutable(BufferTarget::INDEX, cpubuf.size(), cpubuf);
}

void vg::CPUIndexBufferBlock::init_mutable(GLuint i, GLsizei count)
{
	auto& idt_cpubuf = idt_cpubufs[i];
	idt_cpubuf.second.resize(count * index_data_type_size(idt_cpubuf.first));
	bind(i);
	buffers::init_mutable(BufferTarget::INDEX, idt_cpubuf.second.size(), idt_cpubuf.second);
}

void vg::CPUIndexBufferBlock::init_mutable(GLuint i)
{
	bind(i);
	const auto& cpubuf = idt_cpubufs[i].second;
	buffers::init_mutable(BufferTarget::INDEX, cpubuf.size(), cpubuf);
}

void vg::CPUIndexBufferBlock::init_immutable_quads(GLuint i, GLuint num_quads)
{
	auto& idt_cpubuf = idt_cpubufs[i];
	idt_cpubuf.second.resize(num_quads * 6 * index_data_type_size(idt_cpubuf.first));

	if (idt_cpubuf.first == IndexDataType::UBYTE)
		fill_quad_indexes<GLubyte>(idt_cpubuf.second, num_quads);
	else if (idt_cpubuf.first == IndexDataType::USHORT)
		fill_quad_indexes<GLushort>(idt_cpubuf.second, num_quads);
	else if (idt_cpubuf.first == IndexDataType::UINT)
		fill_quad_indexes<GLuint>(idt_cpubuf.second, num_quads);

	init_immutable(i);
}

void vg::CPUIndexBufferBlock::init_mutable_quads(GLuint i, GLuint num_quads)
{
	auto& idt_cpubuf = idt_cpubufs[i];
	idt_cpubuf.second.resize(num_quads * 6 * index_data_type_size(idt_cpubuf.first));

	if (idt_cpubuf.first == IndexDataType::UBYTE)
		fill_quad_indexes<GLubyte>(idt_cpubuf.second, num_quads);
	else if (idt_cpubuf.first == IndexDataType::USHORT)
		fill_quad_indexes<GLushort>(idt_cpubuf.second, num_quads);
	else if (idt_cpubuf.first == IndexDataType::UINT)
		fill_quad_indexes<GLuint>(idt_cpubuf.second, num_quads);

	init_mutable(i);
}
