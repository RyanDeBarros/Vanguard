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
	else if (type != DataType::FLOAT && type != DataType::HALF && integer_case == IntegerCase::INTEGER)
		glVertexAttribIPointer(i, rows, type_as_gl_enum(), stride, (void*)offset);
	else
		glVertexAttribPointer(i, rows, type_as_gl_enum(), integer_case == IntegerCase::NORMALIZED_FLOAT, stride, (void*)offset);
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
	else if (type != DataType::FLOAT && type != DataType::HALF && integer_case == IntegerCase::INTEGER)
		glVertexAttribIPointer(i, rows, type_as_gl_enum(), stride, (void*)offset);
	else
		glVertexAttribPointer(i, rows, type_as_gl_enum(), integer_case == IntegerCase::NORMALIZED_FLOAT, stride, (void*)offset);
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

vg::VertexBufferLayout::VertexBufferLayout(const raii::Shader& shader)
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

vg::VertexBufferLayout::VertexBufferLayout(const raii::Shader& shader, const VertexAttributeSpecificationList& specifications)
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
	for (auto [i, integer_case] : specifications.integer_cases)
		_attributes[i].set_integer_case(integer_case);
	for (auto [i, instance_divisor] : specifications.instance_divisor)
		_attributes[i].set_instance_divisor(instance_divisor);
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

void vg::VertexBuffer::init_immutable_cpu_buffer(VoidArray& cpubuf, GLuint vertex_count) const
{
	cpubuf.resize(_layout->stride() * vertex_count);
	bind_vb();
	buffers::init_immutable(BufferTarget::VERTEX, cpubuf.size());
}

vg::VoidArray vg::VertexBuffer::init_mutable_cpu_buffer(GLuint vertex_count) const
{
	VoidArray v(_layout->stride() * vertex_count);
	bind_vb();
	buffers::init_mutable(BufferTarget::VERTEX, v.size());
	return v;
}

void vg::VertexBuffer::init_mutable_cpu_buffer(VoidArray& cpubuf, GLuint vertex_count) const
{
	cpubuf.resize(_layout->stride() * vertex_count);
	bind_vb();
	buffers::init_mutable(BufferTarget::VERTEX, cpubuf.size());
}

void vg::VertexBufferBlock::init(const std::initializer_list<std::initializer_list<GLuint>>& attributes)
{
	bind_vao();
	_offsets.resize(_layout->attributes().size());
	_strides.resize(_vbs.get_count());
	for (GLuint i = 0; i < _vbs.get_count(); ++i)
	{
		const std::initializer_list<GLuint>& subattributes = *(attributes.begin() + i);
		bind_vb(i);
		const auto& lattrs = _layout->attributes();
		GLuint stride = 0;
		for (GLuint attrib : subattributes)
			stride += lattrs[attrib].bytes();
		_strides[i] = stride;

		GLuint offset = 0;
		for (GLuint attrib : subattributes)
		{
			lattrs[attrib].attrib_pointer(attrib, stride, offset);
			_offsets[attrib] = offset;
			offset += lattrs[attrib].bytes();
		}
	}
	unbind_vertex_array();
}

vg::VertexBufferBlock::VertexBufferBlock(const std::shared_ptr<VertexBufferLayout>& layout, const std::initializer_list<std::initializer_list<GLuint>>& attributes)
	: _layout(layout), _vbs((GLuint)attributes.size())
{
	init(attributes);
}

vg::VertexBufferBlock::VertexBufferBlock(std::shared_ptr<VertexBufferLayout>&& layout, const std::initializer_list<std::initializer_list<GLuint>>& attributes)
	: _layout(std::move(layout)), _vbs((GLuint)attributes.size())
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

void vg::VertexBufferBlock::init_immutable_cpu_buffer(VoidArray& cpubuf, GLuint i, GLuint vertex_count) const
{
	cpubuf.resize(vb_stride(i) * vertex_count);
	bind_vb(i);
	buffers::init_immutable(BufferTarget::VERTEX, cpubuf.size());
}

vg::VoidArray vg::VertexBufferBlock::init_mutable_cpu_buffer(GLuint i, GLuint vertex_count) const
{
	VoidArray v(vb_stride(i) * vertex_count);
	bind_vb(i);
	buffers::init_mutable(BufferTarget::VERTEX, v.size());
	return v;
}

void vg::VertexBufferBlock::init_mutable_cpu_buffer(VoidArray& cpubuf, GLuint i, GLuint vertex_count) const
{
	cpubuf.resize(vb_stride(i) * vertex_count);
	bind_vb(i);
	buffers::init_mutable(BufferTarget::VERTEX, cpubuf.size());
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

vg::MultiVertexBuffer::MultiVertexBuffer(const std::shared_ptr<VertexBufferLayout>& layout, GLuint block_count)
	: _layouts(block_count, layout), _vaos(block_count), _vbs(block_count)
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

void vg::MultiVertexBuffer::init_immutable_cpu_buffer(VoidArray& cpubuf, GLuint i, GLuint vertex_count) const
{
	cpubuf.resize(_layouts[i]->stride() * vertex_count);
	bind_vb(i);
	buffers::init_immutable(BufferTarget::VERTEX, cpubuf.size());
}

vg::VoidArray vg::MultiVertexBuffer::init_mutable_cpu_buffer(GLuint i, GLuint vertex_count) const
{
	VoidArray v(_layouts[i]->stride() * vertex_count);
	bind_vb(i);
	buffers::init_mutable(BufferTarget::VERTEX, v.size());
	return v;
}

void vg::MultiVertexBuffer::init_mutable_cpu_buffer(VoidArray& cpubuf, GLuint i, GLuint vertex_count) const
{
	cpubuf.resize(_layouts[i]->stride() * vertex_count);
	bind_vb(i);
	buffers::init_mutable(BufferTarget::VERTEX, cpubuf.size());
}

// TODO make extern and specify it's for GL_TRIANGLES. Implement for GL_TRIANGLE_STRIP, GL_LINES, etc.

static std::array<GLuint, 6> quad_indexes{ 0, 1, 2, 2, 3, 0 };

template<typename DataType>
static void fill_quad_indexes(vg::VoidArray& cpubuf, GLuint num_quads)
{
	for (GLuint i = 0; i < num_quads; ++i)
		for (GLuint j = 0; j < 6; ++j)
			cpubuf.ref<DataType>((j + 6 * i) * sizeof(DataType)) = quad_indexes[j] + 4 * i;
}

static std::array<GLuint, 36> cube_indexes{
	// Front face
	0, 1, 2, 2, 3, 0,
	// Back face
	4, 5, 6, 6, 7, 4,
	// Left face
	4, 0, 3, 3, 7, 4,
	// Right face
	1, 5, 6, 6, 2, 1,
	// Top face
	3, 2, 6, 6, 7, 3,
	// Bottom face
	4, 5, 1, 1, 0, 4
};

template<typename DataType>
static void fill_cube_indexes(vg::VoidArray& cpubuf, GLuint num_cubes)
{
	for (GLuint i = 0; i < num_cubes; ++i)
		for (GLuint j = 0; j < 36; ++j)
			cpubuf.ref<DataType>((j + 36 * i) * sizeof(DataType)) = cube_indexes[j] + 8 * i;
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

void vg::CPUIndexBuffer::init_immutable_cubes(GLuint num_cubes)
{
	cpubuf.resize(num_cubes * 6 * 6 * index_data_type_size(idt));

	if (idt == IndexDataType::UBYTE)
		fill_cube_indexes<GLubyte>(cpubuf, num_cubes);
	else if (idt == IndexDataType::USHORT)
		fill_cube_indexes<GLushort>(cpubuf, num_cubes);
	else if (idt == IndexDataType::UINT)
		fill_cube_indexes<GLuint>(cpubuf, num_cubes);

	init_immutable();
}

void vg::CPUIndexBuffer::init_mutable_cubes(GLuint num_cubes)
{
	cpubuf.resize(num_cubes * 6 * 6 * index_data_type_size(idt));

	if (idt == IndexDataType::UBYTE)
		fill_cube_indexes<GLubyte>(cpubuf, num_cubes);
	else if (idt == IndexDataType::USHORT)
		fill_cube_indexes<GLushort>(cpubuf, num_cubes);
	else if (idt == IndexDataType::UINT)
		fill_cube_indexes<GLuint>(cpubuf, num_cubes);

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

void vg::CPUIndexBufferBlock::init_immutable_cubes(GLuint i, GLuint num_cubes)
{
	auto& idt_cpubuf = idt_cpubufs[i];
	idt_cpubuf.second.resize(num_cubes * 6 * 6 * index_data_type_size(idt_cpubuf.first));

	if (idt_cpubuf.first == IndexDataType::UBYTE)
		fill_cube_indexes<GLubyte>(idt_cpubuf.second, num_cubes);
	else if (idt_cpubuf.first == IndexDataType::USHORT)
		fill_cube_indexes<GLushort>(idt_cpubuf.second, num_cubes);
	else if (idt_cpubuf.first == IndexDataType::UINT)
		fill_cube_indexes<GLuint>(idt_cpubuf.second, num_cubes);

	init_immutable(i);
}

void vg::CPUIndexBufferBlock::init_mutable_cubes(GLuint i, GLuint num_cubes)
{
	auto& idt_cpubuf = idt_cpubufs[i];
	idt_cpubuf.second.resize(num_cubes * 6 * 6 * index_data_type_size(idt_cpubuf.first));

	if (idt_cpubuf.first == IndexDataType::UBYTE)
		fill_cube_indexes<GLubyte>(idt_cpubuf.second, num_cubes);
	else if (idt_cpubuf.first == IndexDataType::USHORT)
		fill_cube_indexes<GLushort>(idt_cpubuf.second, num_cubes);
	else if (idt_cpubuf.first == IndexDataType::UINT)
		fill_cube_indexes<GLuint>(idt_cpubuf.second, num_cubes);

	init_mutable(i);
}

vg::CPUVertexBuffer::CPUVertexBuffer(VertexBuffer&& vb, GLuint vertex_count, bool is_mutable)
	: _vb(std::move(vb)), _vertex_count(vertex_count)
{
	if (is_mutable)
		_vb.init_mutable_cpu_buffer(_cpubuf, _vertex_count);
	else
		_vb.init_immutable_cpu_buffer(_cpubuf, _vertex_count);
}

void vg::CPUVertexBuffer::subsend_full() const
{
	buffers::subsend(BufferTarget::VERTEX, 0, _cpubuf.size(), _cpubuf);
}

void vg::CPUVertexBuffer::subsend(size_t offset, size_t bytes) const
{
	buffers::subsend(BufferTarget::VERTEX, offset, bytes, _cpubuf.at(offset));
}

void vg::CPUVertexBuffer::subsend_single(GLuint vertex) const
{
	GLintptr offset = buffer_offset(vertex, 0);
	GLuint stride = _vb.layout()->stride();
	buffers::subsend(BufferTarget::VERTEX, offset, stride, _cpubuf.at(offset));
}

void vg::CPUVertexBuffer::subsend_single(GLuint vertex, GLuint attrib) const
{
	GLintptr offset = buffer_offset(vertex, attrib);
	GLuint size = _vb.layout()->attributes()[attrib].bytes();
	buffers::subsend(BufferTarget::VERTEX, offset, size, _cpubuf.at(offset));
}

void vg::CPUVertexBuffer::subsend_single(GLuint vertex, GLuint attrib, GLuint size) const
{
	GLintptr offset = buffer_offset(vertex, attrib);
	buffers::subsend(BufferTarget::VERTEX, offset, size, _cpubuf.at(offset));
}

vg::CPUVertexBufferBlock::CPUVertexBufferBlock(VertexBufferBlock&& vbb, const std::vector<GLuint>& vertex_counts, const std::vector<bool>& is_mutables)
	: _vbb(std::move(vbb))
{
	VANGUARD_ASSERT(_vbb.block_count() == vertex_counts.size() && _vbb.block_count() == is_mutables.size());
	for (GLuint i = 0; i < _vbb.block_count(); ++i)
	{
		if (is_mutables[i])
			_cpubuf_and_vcs.push_back({ _vbb.init_mutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
		else
			_cpubuf_and_vcs.push_back({ _vbb.init_immutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
	}
}

vg::CPUVertexBufferBlock::CPUVertexBufferBlock(VertexBufferBlock&& vbb, GLuint vertex_count, const std::vector<bool>& is_mutables)
	: _vbb(std::move(vbb))
{
	VANGUARD_ASSERT(_vbb.block_count() == is_mutables.size());
	for (GLuint i = 0; i < _vbb.block_count(); ++i)
	{
		if (is_mutables[i])
			_cpubuf_and_vcs.push_back({ _vbb.init_mutable_cpu_buffer(i, vertex_count), vertex_count });
		else
			_cpubuf_and_vcs.push_back({ _vbb.init_immutable_cpu_buffer(i, vertex_count), vertex_count });
	}
}

vg::CPUVertexBufferBlock::CPUVertexBufferBlock(VertexBufferBlock&& vbb, const std::vector<GLuint>& vertex_counts, bool is_mutable)
	: _vbb(std::move(vbb))
{
	VANGUARD_ASSERT(_vbb.block_count() == vertex_counts.size());
	if (is_mutable)
	{
		for (GLuint i = 0; i < _vbb.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbb.init_mutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
	}
	else
	{
		for (GLuint i = 0; i < _vbb.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbb.init_immutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
	}
}

vg::CPUVertexBufferBlock::CPUVertexBufferBlock(VertexBufferBlock&& vbb, GLuint vertex_count, bool is_mutable)
	: _vbb(std::move(vbb))
{
	if (is_mutable)
	{
		for (GLuint i = 0; i < _vbb.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbb.init_mutable_cpu_buffer(i, vertex_count), vertex_count });
	}
	else
	{
		for (GLuint i = 0; i < _vbb.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbb.init_immutable_cpu_buffer(i, vertex_count), vertex_count });
	}
}

void vg::CPUVertexBufferBlock::subsend_full(GLuint i) const
{
	buffers::subsend(BufferTarget::VERTEX, 0, _cpubuf_and_vcs[i].first.size(), _cpubuf_and_vcs[i].first);
}

void vg::CPUVertexBufferBlock::subsend_all_blocks() const
{
	for (GLuint i = 0; i < block_count(); ++i)
	{
		bind_vb(i);
		subsend_full(i);
	}
}

void vg::CPUVertexBufferBlock::subsend(GLuint i, size_t offset, size_t bytes) const
{
	buffers::subsend(BufferTarget::VERTEX, offset, bytes, _cpubuf_and_vcs[i].first.at(offset));
}

void vg::CPUVertexBufferBlock::subsend_single(GLuint i, GLuint vertex, GLuint attrib) const
{
	GLintptr offset = buffer_offset(i, vertex, attrib);
	GLuint size = _vbb.layout()->attributes()[attrib].bytes();
	buffers::subsend(BufferTarget::VERTEX, offset, size, _cpubuf_and_vcs[i].first.at(offset));
}

void vg::CPUVertexBufferBlock::subsend_single(GLuint i, GLuint vertex, GLuint attrib, GLuint size) const
{
	GLintptr offset = buffer_offset(i, vertex, attrib);
	buffers::subsend(BufferTarget::VERTEX, offset, size, _cpubuf_and_vcs[i].first.at(offset));
}

vg::MultiCPUVertexBuffer::MultiCPUVertexBuffer(MultiVertexBuffer&& vbs, const std::vector<GLuint>& vertex_counts, const std::vector<bool>& is_mutables)
	: _vbs(std::move(vbs))
{
	for (GLuint i = 0; i < _vbs.block_count(); ++i)
	{
		if (is_mutables[i])
			_cpubuf_and_vcs.push_back({ _vbs.init_mutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
		else
			_cpubuf_and_vcs.push_back({ _vbs.init_immutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
	}
}

vg::MultiCPUVertexBuffer::MultiCPUVertexBuffer(MultiVertexBuffer&& vbs, GLuint vertex_count, const std::vector<bool>& is_mutables)
	: _vbs(std::move(vbs))
{
	for (GLuint i = 0; i < _vbs.block_count(); ++i)
	{
		if (is_mutables[i])
			_cpubuf_and_vcs.push_back({ _vbs.init_mutable_cpu_buffer(i, vertex_count), vertex_count });
		else
			_cpubuf_and_vcs.push_back({ _vbs.init_immutable_cpu_buffer(i, vertex_count), vertex_count });
	}
}

vg::MultiCPUVertexBuffer::MultiCPUVertexBuffer(MultiVertexBuffer&& vbs, const std::vector<GLuint>& vertex_counts, bool is_mutable)
	: _vbs(std::move(vbs))
{
	if (is_mutable)
	{
		for (GLuint i = 0; i < _vbs.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbs.init_mutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
	}
	else
	{
		for (GLuint i = 0; i < _vbs.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbs.init_immutable_cpu_buffer(i, vertex_counts[i]), vertex_counts[i] });
	}
}

vg::MultiCPUVertexBuffer::MultiCPUVertexBuffer(MultiVertexBuffer&& vbs, GLuint vertex_count, bool is_mutable)
	: _vbs(std::move(vbs))
{
	if (is_mutable)
	{
		for (GLuint i = 0; i < _vbs.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbs.init_mutable_cpu_buffer(i, vertex_count), vertex_count });
	}
	else
	{
		for (GLuint i = 0; i < _vbs.block_count(); ++i)
			_cpubuf_and_vcs.push_back({ _vbs.init_immutable_cpu_buffer(i, vertex_count), vertex_count });
	}
}

void vg::MultiCPUVertexBuffer::subsend_full(GLuint i) const
{
	buffers::subsend(BufferTarget::VERTEX, 0, _cpubuf_and_vcs[i].first.size(), _cpubuf_and_vcs[i].first);
}

void vg::MultiCPUVertexBuffer::subsend_all_blocks() const
{
	for (GLuint i = 0; i < block_count(); ++i)
	{
		bind_vb(i);
		subsend_full(i);
	}
}

void vg::MultiCPUVertexBuffer::subsend(GLuint i, size_t offset, size_t bytes) const
{
	buffers::subsend(BufferTarget::VERTEX, offset, bytes, _cpubuf_and_vcs[i].first.at(offset));
}

void vg::MultiCPUVertexBuffer::subsend_single(GLuint i, GLuint vertex) const
{
	GLintptr offset = buffer_offset(i, vertex, 0);
	GLuint stride = _vbs.layout(i)->stride();
	buffers::subsend(BufferTarget::VERTEX, offset, stride, _cpubuf_and_vcs[i].first.at(offset));
}

void vg::MultiCPUVertexBuffer::subsend_single(GLuint i, GLuint vertex, GLuint attrib) const
{
	GLintptr offset = buffer_offset(i, vertex, attrib);
	GLuint size = _vbs.layout(i)->attributes()[attrib].bytes();
	buffers::subsend(BufferTarget::VERTEX, offset, size, _cpubuf_and_vcs[i].first.at(offset));
}

void vg::MultiCPUVertexBuffer::subsend_single(GLuint i, GLuint vertex, GLuint attrib, GLuint size) const
{
	GLintptr offset = buffer_offset(i, vertex, attrib);
	buffers::subsend(BufferTarget::VERTEX, offset, size, _cpubuf_and_vcs[i].first.at(offset));
}

vg::CompactVBIndexer::CompactVBIndexer(const std::vector<GLuint>& vertex_counts)
{
	indexes.reserve(vertex_counts.size());
	GLuint offset = 0;
	for (GLuint vc : vertex_counts)
	{
		indexes.push_back({ vc, offset });
		offset += vc;
	}
}

void vg::CompactVBIndexer::push_back(GLuint vc)
{
	GLuint offset = vertex_count();
	indexes.push_back({ vc, offset });
}

void vg::CompactVBIndexer::insert(GLuint pos, GLuint vc)
{
	if (pos == indexes.size() - 1)
	{
		GLuint offset = vertex_count();
		indexes.push_back({ vc, offset });
	}
	else if (pos < indexes.size() - 1)
	{
		GLuint offset = indexes[pos].vertex_offset;
		indexes.insert(indexes.begin() + pos, { vc, offset });
		for (auto iter = indexes.begin() + pos + 1; iter != indexes.end(); ++iter)
			iter->vertex_offset += vc;
	}
}

void vg::CompactVBIndexer::erase(GLuint pos)
{
	GLuint vc = indexes[pos].vertex_count;
	indexes.erase(indexes.begin() + pos);
	for (auto iter = indexes.begin() + pos; iter != indexes.end(); ++iter)
		iter->vertex_offset -= vc;
}

void vg::CompactVBIndexer::swap(GLuint pos1, GLuint pos2)
{
	if (pos2 < pos1)
		std::swap(pos1, pos2);
	else if (pos1 == pos2)
		return;

	GLint vc_diff = (GLint)indexes[pos2].vertex_count - (GLint)indexes[pos1].vertex_count;
	std::swap(indexes[pos1].vertex_count, indexes[pos2].vertex_count);
	for (GLuint i = pos1 + 1; i <= pos2; ++i)
		indexes[i].vertex_offset += vc_diff;
}

vg::CompactVBBlockIndexer::CompactVBBlockIndexer(const std::vector<std::vector<GLuint>>& vertex_counts_per_block)
{
	indexes.reserve(vertex_counts_per_block.size());
	for (const auto& vcs : vertex_counts_per_block)
	{
		std::vector<IndexedVB> block_indexes;
		block_indexes.reserve(vcs.size());
		GLuint offset = 0;
		for (GLuint vc : vcs)
		{
			block_indexes.push_back({ vc, offset });
			offset += vc;
		}
		indexes.push_back(std::move(block_indexes));
	}
}

void vg::CompactVBBlockIndexer::push_back(GLuint block, GLuint vc)
{
	GLuint offset = vertex_count(block);
	indexes[block].push_back({ vc, offset });
}

void vg::CompactVBBlockIndexer::insert(GLuint block, GLuint pos, GLuint vc)
{
	auto& block_indexes = indexes[block];
	if (pos == block_indexes.size() - 1)
	{
		GLuint offset = vertex_count(block);
		block_indexes.push_back({ vc, offset });
	}
	else if (pos < block_indexes.size() - 1)
	{
		GLuint offset = block_indexes[pos].vertex_offset;
		block_indexes.insert(block_indexes.begin() + pos, { vc, offset });
		for (auto iter = block_indexes.begin() + pos + 1; iter != block_indexes.end(); ++iter)
			iter->vertex_offset += vc;
	}
}

void vg::CompactVBBlockIndexer::erase(GLuint block, GLuint pos)
{
	auto& block_indexes = indexes[block];
	GLuint vc = block_indexes[pos].vertex_count;
	block_indexes.erase(block_indexes.begin() + pos);
	for (auto iter = block_indexes.begin() + pos; iter != block_indexes.end(); ++iter)
		iter->vertex_offset -= vc;
}

void vg::CompactVBBlockIndexer::swap(GLuint block, GLuint pos1, GLuint pos2)
{
	if (pos2 < pos1)
		std::swap(pos1, pos2);
	else if (pos1 == pos2)
		return;

	auto& block_indexes = indexes[block];
	GLint vc_diff = (GLint)block_indexes[pos2].vertex_count - (GLint)block_indexes[pos1].vertex_count;
	std::swap(block_indexes[pos1].vertex_count, block_indexes[pos2].vertex_count);
	for (GLuint i = pos1 + 1; i <= pos2; ++i)
		block_indexes[i].vertex_offset += vc_diff;
}
