#include "GLBuffer.h"

#include "Vanguard.h"
#include "Errors.h"

vg::raii::GLBuffer::GLBuffer()
{
	glGenBuffers(1, (GLuint*)&_b);
}

vg::raii::GLBuffer::GLBuffer(GLBuffer&& other) noexcept
	: _b(other._b)
{
	other._b = B(0);
}

vg::raii::GLBuffer& vg::raii::GLBuffer::operator=(GLBuffer&& other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(1, (GLuint*)&_b);
		_b = other._b;
		other._b = B(0);
	}
	return *this;
}

vg::raii::GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, (GLuint*)&_b);
}

vg::raii::GLBufferBlock::GLBufferBlock(GLuint count)
	: count(count)
{
	_bs = new B[count];
	glGenBuffers(count, (GLuint*)_bs);
}

vg::raii::GLBufferBlock::GLBufferBlock(GLBufferBlock&& other) noexcept
	: _bs(other._bs), count(other.count)
{
	other._bs = nullptr;
	other.count = 0;
}

vg::raii::GLBufferBlock& vg::raii::GLBufferBlock::operator=(GLBufferBlock&& other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(count, (GLuint*)_bs);
		delete[] _bs;
		_bs = other._bs;
		other._bs = nullptr;
		count = other.count;
		other.count = 0;
	}
	return *this;
}

vg::raii::GLBufferBlock::~GLBufferBlock()
{
	glDeleteBuffers(count, (GLuint*)_bs);
	delete[] _bs;
}

vg::ids::GLBuffer vg::raii::GLBufferBlock::operator[](GLuint i) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	return _bs[i];
}

vg::raii::VertexArray::VertexArray()
	: block(2)
{
	glGenVertexArrays(1, (GLuint*)&_vao);
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	glVertexArrayElementBuffer(_vao, ib());
#else
	glBindVertexArray(_vao);
	buffers::bind(ib(), BufferTarget::INDEX);
#endif
	glBindVertexArray(0);
}

vg::raii::VertexArray::VertexArray(VertexArray&& other) noexcept
	: _vao(other._vao), block(std::move(other.block))
{
	other._vao = V(0);
}

vg::raii::VertexArray& vg::raii::VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, (GLuint*)&_vao);
		_vao = other._vao;
		other._vao = V(0);
		block = std::move(other.block);
	}
	return *this;
}

vg::raii::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, (GLuint*)&_vao);
}

vg::ids::GLBuffer vg::raii::VertexArray::vb() const
{
	return block[0];
}

vg::ids::GLBuffer vg::raii::VertexArray::ib() const
{
	return block[1];
}

void vg::raii::VertexArray::bind() const
{
	glBindVertexArray(_vao);
	buffers::bind(vb(), BufferTarget::VERTEX);
}

vg::raii::VertexArrayBlock::VertexArrayBlock(GLuint count)
	: count(count), block(count * 2)
{
	_vaos = new V[count];
	glGenVertexArrays(count, (GLuint*)_vaos);
	for (GLuint i = 0; i < count; ++i)
	{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		glVertexArrayElementBuffer(_vaos[i], ib(i));
#else
		glBindVertexArray(_vaos[i]);
		buffers::bind(ib(i), BufferTarget::INDEX);
#endif
	}
	glBindVertexArray(0);
}

vg::raii::VertexArrayBlock::VertexArrayBlock(VertexArrayBlock&& other) noexcept
	: count(other.count), block(std::move(other.block)), _vaos(other._vaos)
{
	other.count = 0;
	other._vaos = nullptr;
}

vg::raii::VertexArrayBlock& vg::raii::VertexArrayBlock::operator=(VertexArrayBlock&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(count, (GLuint*)_vaos);
		delete[] _vaos;
		_vaos = other._vaos;
		other._vaos = nullptr;
		count = other.count;
		other.count = 0;
	}
	return *this;
}

vg::raii::VertexArrayBlock::~VertexArrayBlock()
{
	glDeleteVertexArrays(count, (GLuint*)_vaos);
	delete[] _vaos;
}

vg::ids::VertexArray vg::raii::VertexArrayBlock::operator[](GLuint i) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	return _vaos[i];
}

vg::ids::GLBuffer vg::raii::VertexArrayBlock::vb(GLuint i) const
{
	return block[i];
}

vg::ids::GLBuffer vg::raii::VertexArrayBlock::ib(GLuint i) const
{
	return block[i + count];
}

void vg::raii::VertexArrayBlock::bind(GLuint i) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	glBindVertexArray(_vaos[i]);
	buffers::bind(vb(i), BufferTarget::VERTEX);
}

GLintptr vg::index_data_type_size(IndexDataType idt)
{
	if (idt == IndexDataType::UBYTE)
		return sizeof(unsigned char);
	else if (idt == IndexDataType::USHORT)
		return sizeof(unsigned short);
	else if (idt == IndexDataType::UINT)
		return sizeof(unsigned int);
	else
		return 0;
}

void vg::unbind_vertex_array()
{
	buffers::unbind(BufferTarget::VERTEX);
	glBindVertexArray(0);
}

vg::GPUIndirectArrays::GPUIndirectArrays()
{
	bind();
	buffers::init_immutable(BufferTarget::DRAW_INDIRECT, sizeof(IndirectArraysCmd));
}

void vg::GPUIndirectArrays::bind() const
{
	buffers::bind(b, BufferTarget::DRAW_INDIRECT);
}

void vg::GPUIndirectArrays::draw(DrawMode mode) const
{
	glDrawArraysIndirect((GLenum)mode, (void*)0);
}

void vg::GPUIndirectArrays::send_vertex_count(GLuint vertex_count) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 0, sizeof(GLuint), &vertex_count);
}

void vg::GPUIndirectArrays::send_instance_count(GLuint instance_count) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, sizeof(GLuint), sizeof(GLuint), &instance_count);
}

void vg::GPUIndirectArrays::send_first_vertex(GLuint first_vertex) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 2 * sizeof(GLuint), sizeof(GLuint), &first_vertex);
}

void vg::GPUIndirectArrays::send_first_instance(GLuint first_instance) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 3 * sizeof(GLuint), sizeof(GLuint), &first_instance);
}

void vg::GPUIndirectArrays::send_cmd(IndirectArraysCmd cmd) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 0, sizeof(IndirectArraysCmd), &cmd);
}

vg::GPUIndirectArraysBlock::GPUIndirectArraysBlock(GLuint count)
	: count(count)
{
	bind();
	buffers::init_immutable(BufferTarget::DRAW_INDIRECT, count * sizeof(IndirectArraysCmd));
}

void vg::GPUIndirectArraysBlock::bind() const
{
	buffers::bind(b, BufferTarget::DRAW_INDIRECT);
}

void vg::GPUIndirectArraysBlock::draw(GLuint i, DrawMode mode) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	glDrawArraysIndirect((GLenum)mode, (void*)(i * sizeof(IndirectArraysCmd)));
}

void vg::GPUIndirectArraysBlock::multi_draw(DrawMode mode, GLuint first, GLuint count) const
{
	GLuint last = first + count - 1;
	if (last >= this->count)
		throw block_index_out_of_range(this->count, last);
	glMultiDrawArraysIndirect((GLenum)mode, (void*)(first * sizeof(IndirectArraysCmd)), count, sizeof(IndirectArraysCmd));
}

void vg::GPUIndirectArraysBlock::send_vertex_count(GLuint i, GLuint vertex_count) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, i * sizeof(IndirectArraysCmd), sizeof(GLuint), &vertex_count);
}

void vg::GPUIndirectArraysBlock::send_instance_count(GLuint i, GLuint instance_count) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, sizeof(GLuint) + i * sizeof(IndirectArraysCmd), sizeof(GLuint), &instance_count);
}

void vg::GPUIndirectArraysBlock::send_first_vertex(GLuint i, GLuint first_vertex) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 2 * sizeof(GLuint) + i * sizeof(IndirectArraysCmd), sizeof(GLuint), &first_vertex);
}

void vg::GPUIndirectArraysBlock::send_first_instance(GLuint i, GLuint first_instance) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 3 * sizeof(GLuint) + i * sizeof(IndirectArraysCmd), sizeof(GLuint), &first_instance);
}

void vg::GPUIndirectArraysBlock::send_cmd(GLuint i, IndirectArraysCmd cmd) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, i * sizeof(IndirectArraysCmd), sizeof(IndirectArraysCmd), &cmd);
}

void vg::GPUIndirectArraysBlock::send_cmds(GLuint first, GLuint count, IndirectArraysCmd* cmds) const
{
	GLuint last = first + count - 1;
	if (last >= this->count)
		throw block_index_out_of_range(this->count, last);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, first * sizeof(IndirectArraysCmd), count * sizeof(IndirectArraysCmd), cmds);
}

vg::GPUIndirectElements::GPUIndirectElements()
{
	bind();
	buffers::init_immutable(BufferTarget::DRAW_INDIRECT, sizeof(IndirectElementsCmd));
}

void vg::GPUIndirectElements::bind() const
{
	buffers::bind(b, BufferTarget::DRAW_INDIRECT);
}

void vg::GPUIndirectElements::draw(DrawMode mode, IndexDataType idt) const
{
	glDrawElementsIndirect((GLenum)mode, (GLenum)idt, 0);
}

void vg::GPUIndirectElements::send_index_count(GLuint index_count) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 0, sizeof(GLuint), &index_count);
}

void vg::GPUIndirectElements::send_instance_count(GLuint instance_count) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, sizeof(GLuint), sizeof(GLuint), &instance_count);
}

void vg::GPUIndirectElements::send_first_index(GLuint first_index) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 2 * sizeof(GLuint), sizeof(GLuint), &first_index);
}

void vg::GPUIndirectElements::send_base_vertex(GLuint base_vertex) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 3 * sizeof(GLuint), sizeof(GLuint), &base_vertex);
}

void vg::GPUIndirectElements::send_first_instance(GLuint first_instance) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 4 * sizeof(GLuint), sizeof(GLuint), &first_instance);
}

void vg::GPUIndirectElements::send_cmd(IndirectElementsCmd cmd) const
{
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 0, sizeof(IndirectElementsCmd), &cmd);
}

vg::GPUIndirectElementsBlock::GPUIndirectElementsBlock(GLuint count)
	: count(count)
{
	bind();
	buffers::init_immutable(BufferTarget::DRAW_INDIRECT, count * sizeof(IndirectElementsCmd));
}

void vg::GPUIndirectElementsBlock::bind() const
{
	buffers::bind(b, BufferTarget::DRAW_INDIRECT);
}

void vg::GPUIndirectElementsBlock::draw(GLuint i, DrawMode mode, IndexDataType idt) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	glDrawElementsIndirect((GLenum)mode, (GLenum)idt, (void*)(i * sizeof(IndirectElementsCmd)));
}

void vg::GPUIndirectElementsBlock::multi_draw(DrawMode mode, GLuint first, GLuint count, IndexDataType idt) const
{
	GLuint last = first + count - 1;
	if (last >= this->count)
		throw block_index_out_of_range(this->count, last);
	glMultiDrawElementsIndirect((GLenum)mode, (GLenum)idt, (void*)(first * sizeof(IndirectElementsCmd)), count, sizeof(IndirectElementsCmd));
}

void vg::GPUIndirectElementsBlock::send_index_count(GLuint i, GLuint index_count) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, i * sizeof(IndirectElementsCmd), sizeof(GLuint), &index_count);
}

void vg::GPUIndirectElementsBlock::send_instance_count(GLuint i, GLuint instance_count) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, sizeof(GLuint) + i * sizeof(IndirectElementsCmd), sizeof(GLuint), &instance_count);
}

void vg::GPUIndirectElementsBlock::send_first_index(GLuint i, GLuint first_index) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 2 * sizeof(GLuint) + i * sizeof(IndirectElementsCmd), sizeof(GLuint), &first_index);
}

void vg::GPUIndirectElementsBlock::send_base_vertex(GLuint i, GLuint base_vertex) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 3 * sizeof(GLuint) + i * sizeof(IndirectElementsCmd), sizeof(GLuint), &base_vertex);
}

void vg::GPUIndirectElementsBlock::send_first_instance(GLuint i, GLuint first_instance) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, 4 * sizeof(GLuint) + i * sizeof(IndirectElementsCmd), sizeof(GLuint), &first_instance);
}

void vg::GPUIndirectElementsBlock::send_cmd(GLuint i, IndirectElementsCmd cmd) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, i * sizeof(IndirectElementsCmd), sizeof(IndirectElementsCmd), &cmd);
}

void vg::GPUIndirectElementsBlock::send_cmds(GLuint first, GLuint count, IndirectElementsCmd* cmds) const
{
	GLuint last = first + count - 1;
	if (last >= this->count)
		throw block_index_out_of_range(this->count, last);
	buffers::subsend(BufferTarget::DRAW_INDIRECT, first * sizeof(IndirectElementsCmd), count * sizeof(IndirectElementsCmd), cmds);
}

void vg::buffers::bind(ids::GLBuffer b, BufferTarget target)
{
	glBindBuffer((GLenum)target, b);
}

void vg::buffers::unbind(BufferTarget target)
{
	glBindBuffer((GLenum)target, 0);
}

void vg::buffers::init_immutable(BufferTarget target, GLsizeiptr size, const void* data, int usage)
{
	glBufferStorage((GLenum)target, size, data, usage);
}

void vg::buffers::init_mutable(BufferTarget target, GLsizeiptr size, const void* data, BufferMutableUsage usage)
{
	glBufferData((GLenum)target, size, data, (GLenum)usage);
}

void vg::buffers::subsend(BufferTarget target, GLintptr offset_bytes, GLsizeiptr size, const void* data)
{
	glBufferSubData((GLenum)target, offset_bytes, size, data);
}

void vg::buffers::map(BufferTarget target, void* data, size_t size)
{
	void* client = glMapBuffer((GLenum)target, GL_WRITE_ONLY);
	memcpy(client, data, size);
	glUnmapBuffer((GLenum)target);
}

void vg::buffers::submap(BufferTarget target, GLintptr offset_bytes, GLsizeiptr length_bytes, void* data)
{
	void* client = glMapBufferRange((GLenum)target, offset_bytes, length_bytes, GL_MAP_WRITE_BIT);
	memcpy(client, data, length_bytes);
	glUnmapBuffer((GLenum)target);
}

void vg::buffers::copy_gl_buffer(ids::GLBuffer b_src, ids::GLBuffer b_dst, GLintptr offset_src_bytes, GLintptr offset_dst_bytes, GLsizeiptr size)
{
	bind(b_src, BufferTarget::COPY_READ);
	bind(b_dst, BufferTarget::COPY_WRITE);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset_src_bytes, offset_dst_bytes, size);
}

void vg::buffers::copy_bound_gl_buffers(GLintptr offset_src_bytes, GLintptr offset_dst_bytes, GLsizeiptr size)
{
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset_src_bytes, offset_dst_bytes, size);
}

vg::VoidArray vg::buffers::read(BufferTarget target, GLintptr offset_bytes, GLsizeiptr size)
{
	VoidArray data(size);
	glGetBufferSubData((GLenum)target, offset_bytes, size, data);
	return data;
}

void vg::draw::arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count)
{
	glDrawArrays((GLenum)mode, first_vertex, vertex_count);
}

void vg::draw::elements(DrawMode mode, GLsizei index_count, GLuint first_index, IndexDataType idt)
{
	glDrawElements((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)));
}

void vg::draw::element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, IndexDataType idt)
{
	glDrawRangeElements((GLenum)mode, minimum_index, maximum_index, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)));
}

void vg::draw::instanced::arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count)
{
	glDrawArraysInstanced((GLenum)mode, first_vertex, vertex_count, instance_count);
}

void vg::draw::instanced::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, IndexDataType idt)
{
	glDrawElementsInstanced((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count);
}

void vg::draw::instanced::arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count, GLuint first_instance)
{
	glDrawArraysInstancedBaseInstance((GLenum)mode, first_vertex, vertex_count, instance_count, first_instance);
}

void vg::draw::instanced::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLuint first_instance, IndexDataType idt)
{
	glDrawElementsInstancedBaseInstance((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count, first_instance);
}

void vg::draw::base_vertex::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt)
{
	glDrawElementsBaseVertex((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), base_vertex);
}

void vg::draw::base_vertex::element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt)
{
	glDrawRangeElementsBaseVertex((GLenum)mode, minimum_index, maximum_index, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), base_vertex);
}

void vg::draw::instanced_base_vertex::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, IndexDataType idt)
{
	glDrawElementsInstancedBaseVertex((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count, base_vertex);
}

void vg::draw::instanced_base_vertex::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, GLuint first_instance, IndexDataType idt)
{
	glDrawElementsInstancedBaseVertexBaseInstance((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count, base_vertex, first_instance);
}

void vg::draw::multi::arrays(DrawMode mode, const GLint* first_vertices, const GLsizei* vertex_counts, GLsizei drawcount)
{
	glMultiDrawArrays((GLenum)mode, first_vertices, vertex_counts, drawcount);
}

void vg::draw::multi::elements(DrawMode mode, const GLsizei* index_counts, const GLsizeiptr* first_index_bytes, IndexDataType idt, GLsizei drawcount)
{
	glMultiDrawElements((GLenum)mode, index_counts, (GLenum)idt, (void**)first_index_bytes, drawcount);
}

void vg::draw::multi::elements_base_vertex(DrawMode mode, GLsizei* index_counts, const GLintptr* first_index_bytes, IndexDataType idt, GLint* base_vertices, GLsizei drawcount)
{
	glMultiDrawElementsBaseVertex((GLenum)mode, index_counts, (GLenum)idt, (void**)first_index_bytes, drawcount, base_vertices);
}
