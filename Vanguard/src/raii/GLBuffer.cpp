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
}

GLuint vg::GLBufferBlock::operator[](GLsizei i) const
{
	if (i < count && i >= 0)
		return _b[i];
	throw block_index_out_of_range(count, i);
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
