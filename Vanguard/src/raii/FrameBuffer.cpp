#include "FrameBuffer.h"

#include "Errors.h"

vg::raii::FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, (GLuint*)&_f);
}

vg::raii::FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
	: _f(other._f)
{
	other._f = ids::FrameBuffer(0);
}

vg::raii::FrameBuffer& vg::raii::FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
	if (this != &other)
	{
		glDeleteFramebuffers(1, (GLuint*)&_f);
		_f = other._f;
		other._f = ids::FrameBuffer(0);
	}
	return *this;
}

vg::raii::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, (GLuint*)&_f);
}

vg::raii::FrameBufferBlock::FrameBufferBlock(GLuint count)
	: count(count)
{
	glGenFramebuffers(count, (GLuint*)_fs);
}

vg::raii::FrameBufferBlock::FrameBufferBlock(FrameBufferBlock&& other) noexcept
	: _fs(other._fs), count(other.count)
{
	other._fs = nullptr;
	other.count = 0;
}

vg::raii::FrameBufferBlock& vg::raii::FrameBufferBlock::operator=(FrameBufferBlock&& other) noexcept
{
	if (this != &other)
	{
		glDeleteFramebuffers(count, (GLuint*)_fs);
		_fs = other._fs;
		other._fs = nullptr;
		count = other.count;
		other.count = 0;
	}
	return *this;
}

vg::raii::FrameBufferBlock::~FrameBufferBlock()
{
	glDeleteFramebuffers(count, (GLuint*)_fs);
}

vg::ids::FrameBuffer vg::raii::FrameBufferBlock::operator[](GLuint i) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	return _fs[i];
}

void vg::bind_framebuffer(ids::FrameBuffer fb, FrameBufferUsage usage)
{
	switch (usage)
	{
	case FrameBufferUsage::DUAL:
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		break;
	case FrameBufferUsage::DRAW:
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
		break;
	case FrameBufferUsage::READ:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
		break;
	}
}

void vg::unbind_framebuffer(FrameBufferUsage usage)
{
	switch (usage)
	{
	case FrameBufferUsage::DUAL:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		break;
	case FrameBufferUsage::DRAW:
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		break;
	case FrameBufferUsage::READ:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		break;
	}
}
