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

void vg::framebuffers::bind(ids::FrameBuffer fb, Target target)
{
	glBindFramebuffer((GLenum)target, fb);
}

void vg::framebuffers::unbind(Target target)
{
	glBindFramebuffer((GLenum)target, 0);
}

void vg::framebuffers::attach_texture(ids::Texture texture, Attachment attachment, Target target)
{
	glFramebufferTexture((GLenum)target, (GLenum)attachment, texture, 0);
}

void vg::framebuffers::attach_texture_layer(ids::Texture texture, Attachment attachment, Target target, GLuint layer)
{
	glFramebufferTextureLayer((GLenum)target, (GLenum)attachment, texture, 0, layer);
}

vg::framebuffers::Status vg::framebuffers::status(Target target)
{
	return (Status)glCheckFramebufferStatus((GLenum)target);
}

bool vg::framebuffers::is_complete(Target target)
{
	return status(target) == Status::COMPLETE;
}

void vg::framebuffers::draw_into(Attachment attachment)
{
	glDrawBuffer((GLenum)attachment);
}

void vg::framebuffers::draw_into(const Attachment* attachments, GLuint count)
{
	glDrawBuffers(count, (const GLenum*)attachments);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)

void vg::framebuffers::attach_texture(ids::FrameBuffer fb, ids::Texture texture, Attachment attachment)
{
	glNamedFramebufferTexture(fb, (GLenum)attachment, texture, 0);
}

void vg::framebuffers::attach_texture(ids::FrameBuffer fb, ids::Texture texture, Attachment attachment, GLuint layer)
{
	glNamedFramebufferTextureLayer(fb, (GLenum)attachment, texture, 0, layer);
}

vg::framebuffers::Status vg::framebuffers::status(ids::FrameBuffer fb, Target target)
{
	return (Status)glCheckNamedFramebufferStatus(fb, (GLenum)target);
}

bool vg::framebuffers::is_complete(ids::FrameBuffer fb, Target target)
{
	return status(fb, target) == Status::COMPLETE;
}

void vg::framebuffers::draw_into(ids::FrameBuffer fb, Attachment attachment)
{
	glNamedFramebufferDrawBuffer(fb, (GLenum)attachment);
}

void vg::framebuffers::draw_into(ids::FrameBuffer fb, const Attachment* attachments, GLuint count)
{
	glNamedFramebufferDrawBuffers(fb, count, (const GLenum*)attachments);
}

#endif

void vg::framebuffers::select_read_mode(ReadBuffer mode)
{
	glReadBuffer((GLenum)mode);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
void vg::framebuffers::select_read_mode(ids::FrameBuffer fb, ReadBuffer mode)
{
	glNamedFramebufferReadBuffer(fb, (GLenum)mode);
}
#endif

void vg::framebuffers::blit(IntBounds src, IntBounds dst, BlitMask mask, MagFilter filter)
{
	glBlitFramebuffer(src.x0, src.y0, src.x1, src.y1, dst.x0, dst.y0, dst.x1, dst.y1, (GLbitfield)mask, (GLenum)filter);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
void vg::framebuffers::blit(ids::FrameBuffer src_fb, ids::FrameBuffer dst_fb, IntBounds src, IntBounds dst, BlitMask mask, MagFilter filter)
{
	glBlitNamedFramebuffer(src_fb, dst_fb, src.x0, src.y0, src.x1, src.y1, dst.x0, dst.y0, dst.x1, dst.y1, (GLbitfield)mask, (GLenum)filter);
}
#endif

void vg::FrameBufferObject::attach_texture(ids::Texture texture, framebuffers::Attachment attachment)
{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	framebuffers::attach_texture(fb, texture, attachment);
#else
	bind();
	framebuffers::attach_texture(texture, attachment, target);
#endif
}

void vg::FrameBufferObject::attach_textures(ids::Texture* textures, const framebuffers::Attachment* attachments, GLuint count)
{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	for (GLuint i = 0; i < count; ++i)
		framebuffers::attach_texture(fb, textures[i], attachments[i]);
#else
	bind();
	for (GLuint i = 0; i < count; ++i)
		framebuffers::attach_texture(textures[i], attachments[i], target);
#endif
}

void vg::FrameBufferObject::draw_into(framebuffers::Attachment attachment) const
{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	framebuffers::draw_into(fb, attachment);
#else
	bind();
	framebuffers::draw_into(attachment);
#endif
}

void vg::FrameBufferObject::draw_into(const framebuffers::Attachment* attachments, GLuint count) const
{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	framebuffers::draw_into(fb, attachments, count);
#else
	bind();
	framebuffers::draw_into(attachments, count);
#endif
}

void vg::FrameBufferObject::begin_render() const
{
	bind();
	glViewport(x, y, width, height);
	vg::set_clear_color(clear_color);
	vg::clear_buffer();
}
