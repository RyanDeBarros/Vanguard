#include "GLBuffer.h"

#include "Errors.h"

vg::GLBuffer::GLBuffer()
{
	glGenBuffers(1, &_b);
}

vg::GLBuffer::GLBuffer(GLBuffer&& other) noexcept
	: _b(other._b)
{
	other._b = 0;
}

vg::GLBuffer& vg::GLBuffer::operator=(GLBuffer&& other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(1, &_b);
		_b = other._b;
		other._b = 0;
	}
	return *this;
}

vg::GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &_b);
}

vg::GLBufferBlock::GLBufferBlock(GLsizei count)
	: count(count)
{
	_b = new GLuint[count];
	glGenBuffers(count, _b);
}

vg::GLBufferBlock::GLBufferBlock(GLBufferBlock&& other) noexcept
	: _b(other._b), count(other.count)
{
	other._b = nullptr;
	other.count = 0;
}

vg::GLBufferBlock& vg::GLBufferBlock::operator=(GLBufferBlock&& other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(count, _b);
		delete[] _b;
		_b = other._b;
		other._b = nullptr;
		count = other.count;
		other.count = 0;
	}
	return *this;
}

vg::GLBufferBlock::~GLBufferBlock()
{
	glDeleteBuffers(count, _b);
	delete[] _b;
}

GLuint vg::GLBufferBlock::operator[](GLsizei i) const
{
	if (i >= count || i < 0)
		throw block_index_out_of_range(count, i);
	return _b[i];
}

vg::VertexArray::VertexArray()
	: block(2), idt(IndexDataType::UBYTE)
{
	glGenVertexArrays(1, &_vao);
}

vg::VertexArray::VertexArray(VertexArray&& other) noexcept
	: _vao(other._vao), block(std::move(other.block)), idt(other.idt)
{
	other._vao = 0;
}

vg::VertexArray& vg::VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(1, &_vao);
		_vao = other._vao;
		other._vao = 0;
		block = std::move(other.block);
		idt = other.idt;
	}
	return *this;
}

vg::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_vao);
}

GLuint vg::VertexArray::vb() const
{
	return block[0];
}

GLuint vg::VertexArray::ib() const
{
	return block[1];
}

void vg::VertexArray::bind() const
{
	glBindVertexArray(_vao);
	vertex_buffer::bind(vb());
	index_buffer::bind(ib());
}

vg::VertexArrayBlock::VertexArrayBlock(GLsizei count)
	: count(count), block(count * 2)
{
	_vaos = new GLuint[count];
	glGenVertexArrays(count, _vaos);
	idts = new IndexDataType[count](IndexDataType::UBYTE);
}

vg::VertexArrayBlock::VertexArrayBlock(VertexArrayBlock&& other) noexcept
	: count(other.count), block(std::move(other.block)), _vaos(other._vaos), idts(other.idts)
{
	other.count = 0;
	other._vaos = nullptr;
	other.idts = nullptr;
}

vg::VertexArrayBlock& vg::VertexArrayBlock::operator=(VertexArrayBlock&& other) noexcept
{
	if (this != &other)
	{
		glDeleteVertexArrays(count, _vaos);
		delete[] _vaos;
		_vaos = other._vaos;
		other._vaos = nullptr;
		delete[] idts;
		idts = other.idts;
		other.idts = nullptr;
		count = other.count;
		other.count = 0;
	}
	return *this;
}

vg::VertexArrayBlock::~VertexArrayBlock()
{
	glDeleteVertexArrays(count, _vaos);
	delete[] _vaos;
	delete[] idts;
}

GLuint vg::VertexArrayBlock::operator[](GLsizei i) const
{
	if (i >= count || i < 0)
		throw block_index_out_of_range(count, i);
	return _vaos[i];
}

GLuint vg::VertexArrayBlock::vb(GLsizei i) const
{
	return block[2 * i];
}

GLuint vg::VertexArrayBlock::ib(GLsizei i) const
{
	return block[2 * i + 1];
}

vg::IndexDataType vg::VertexArrayBlock::index_data_type(GLsizei i) const
{
	if (i >= count || i < 0)
		throw block_index_out_of_range(count, i);
	return idts[i];
}

vg::IndexDataType& vg::VertexArrayBlock::index_data_type(GLsizei i)
{
	if (i >= count || i < 0)
		throw block_index_out_of_range(count, i);
	return idts[i];
}

void vg::VertexArrayBlock::bind(GLsizei i) const
{
	if (i >= count || i < 0)
		throw block_index_out_of_range(count, i);
	glBindVertexArray(_vaos[i]);
	vertex_buffer::bind(vb(i));
	index_buffer::bind(ib(i));
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
	index_buffer::unbind();
	vertex_buffer::unbind();
	glBindVertexArray(0);
}

void vg::vertex_buffer::bind(GLuint vb)
{
	glBindBuffer(GL_ARRAY_BUFFER, vb);
}

void vg::vertex_buffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vg::vertex_buffer::init_immutable(GLuint vb, GLsizeiptr size, const void* data, int usage)
{
	bind(vb);
	glBufferStorage(GL_ARRAY_BUFFER, size, data, usage);
}

void vg::vertex_buffer::init_mutable(GLuint vb, GLsizeiptr size, const void* data, BufferMutableUsage usage)
{
	bind(vb);
	glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
}

void vg::vertex_buffer::subsend(GLuint vb, GLintptr offset, GLsizeiptr size, const void* data)
{
	bind(vb);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void vg::vertex_buffer::map(GLuint vb, void* data, size_t size)
{
	bind(vb);
	void* client = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(client, data, size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void vg::vertex_buffer::submap(GLuint vb, GLintptr offset, GLsizeiptr length, void* data)
{
	bind(vb);
	void* client = glMapBufferRange(GL_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT);
	memcpy(client, data, length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void vg::index_buffer::bind(GLuint ib)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

void vg::index_buffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void vg::index_buffer::init_immutable(GLuint ib, GLsizeiptr size, const void* data, int usage)
{
	bind(ib);
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void vg::index_buffer::init_mutable(GLuint ib, GLsizeiptr size, const void* data, BufferMutableUsage usage)
{
	bind(ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, (GLenum)usage);
}

void vg::index_buffer::subsend(GLuint ib, GLintptr offset, GLsizeiptr size, const void* data)
{
	bind(ib);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void vg::index_buffer::map(GLuint ib, void* data, size_t size)
{
	bind(ib);
	void* client = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(client, data, size);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void vg::index_buffer::submap(GLuint ib, GLintptr offset, GLsizeiptr length, void* data)
{
	bind(ib);
	void* client = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT);
	memcpy(client, data, length);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void vg::copy_gl_buffer(GLuint vb_src, GLuint vb_dst, GLintptr offset_src, GLintptr offset_dst, GLsizeiptr size)
{
	glBindBuffer(GL_COPY_READ_BUFFER, vb_src);
	glBindBuffer(GL_COPY_WRITE_BUFFER, vb_dst);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset_src, offset_dst, size);
}

void vg::draw::arrays(GLuint vb, DrawMode mode, GLint first, GLsizei count)
{
	vertex_buffer::bind(vb);
	glDrawArrays((GLenum)mode, first, count);
}

void vg::draw::elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawElements((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)));
}

void vg::draw::elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawElements((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)));
}

void vg::draw::element_range(const VertexArray& vao, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawRangeElements((GLenum)mode, start, end, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)));
}

void vg::draw::element_range(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawRangeElements((GLenum)mode, start, end, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)));
}

void vg::draw::instanced::arrays(GLuint vb, DrawMode mode, GLint first, GLsizei count, GLsizei primcount)
{
	vertex_buffer::bind(vb);
	glDrawArraysInstanced((GLenum)mode, first, count, primcount);
}

void vg::draw::instanced::arrays(GLuint vb, DrawMode mode, GLint first, GLsizei count, GLsizei primcount, GLuint primoffset)
{
	vertex_buffer::bind(vb);
	glDrawArraysInstancedBaseInstance((GLenum)mode, first, count, primcount, primoffset);
}

void vg::draw::instanced::elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawElementsInstanced((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount);
}

void vg::draw::instanced::elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawElementsInstanced((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount);
}

void vg::draw::instanced::elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLuint primoffset)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawElementsInstancedBaseInstance((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount, primoffset);
}

void vg::draw::instanced::elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLuint primoffset)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawElementsInstancedBaseInstance((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount, primoffset);
}

void vg::draw::instanced::base_vertex::elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawElementsInstancedBaseVertex((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount, base_vertex);
}

void vg::draw::instanced::base_vertex::elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawElementsInstancedBaseVertex((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount, base_vertex);
}

void vg::draw::instanced::base_vertex::elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex, GLuint primoffset)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawElementsInstancedBaseVertexBaseInstance((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount, base_vertex, primoffset);
}

void vg::draw::instanced::base_vertex::elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLsizei primcount, GLint base_vertex, GLuint primoffset)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawElementsInstancedBaseVertexBaseInstance((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), primcount, base_vertex, primoffset);
}

void vg::draw::base_vertex::elements(const VertexArray& vao, DrawMode mode, GLsizei count, GLuint offset, GLint base_vertex)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawElementsBaseVertex((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), base_vertex);
}

void vg::draw::base_vertex::elements(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLsizei count, GLuint offset, GLint base_vertex)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawElementsBaseVertex((GLenum)mode, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), base_vertex);
}

void vg::draw::base_vertex::element_range(const VertexArray& vao, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset, GLint base_vertex)
{
	vao.bind();
	IndexDataType idt = vao.index_data_type();
	glDrawRangeElementsBaseVertex((GLenum)mode, start, end, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), base_vertex);
}

void vg::draw::base_vertex::element_range(const VertexArrayBlock& vao, GLsizei i, DrawMode mode, GLuint start, GLuint end, GLsizei count, GLuint offset, GLint base_vertex)
{
	vao.bind(i);
	IndexDataType idt = vao.index_data_type(i);
	glDrawRangeElementsBaseVertex((GLenum)mode, start, end, count, (GLenum)idt, (void*)(offset * index_data_type_size(idt)), base_vertex);
}
