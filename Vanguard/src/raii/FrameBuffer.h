#pragma once

#include "Vendor.h"
#include "Texture.h"

namespace vg
{
	namespace ids
	{
		class FrameBuffer
		{
			GLuint id;

		public:
			explicit FrameBuffer(GLuint id = 0) : id(id) {}
			operator GLuint () const { return id; }
		};
	}

	namespace raii
	{
		class FrameBuffer
		{
			ids::FrameBuffer _f;

		public:
			FrameBuffer();
			FrameBuffer(const FrameBuffer&) = delete;
			FrameBuffer(FrameBuffer&&) noexcept;
			FrameBuffer& operator=(FrameBuffer&&) noexcept;
			~FrameBuffer();

			operator ids::FrameBuffer() const { return _f; }
		};

		class FrameBufferBlock
		{
			ids::FrameBuffer* _fs = nullptr;
			GLuint count;

		public:
			FrameBufferBlock(GLuint count);
			FrameBufferBlock(const FrameBufferBlock&) = delete;
			FrameBufferBlock(FrameBufferBlock&&) noexcept;
			FrameBufferBlock& operator=(FrameBufferBlock&&) noexcept;
			~FrameBufferBlock();

			ids::FrameBuffer operator[](GLuint i) const;
			GLuint get_count() const { return count; }
		};
	}

	// LATER RenderBuffer objects.

	// TODO named (direct) versions of functions everywhere
	// TODO later, EXT functions.

	namespace framebuffers
	{
		enum class Target : unsigned char
		{
			DUAL,
			DRAW,
			READ
		};

		enum class Attachment
		{
			COLOR0 = GL_COLOR_ATTACHMENT0,
			COLOR1 = GL_COLOR_ATTACHMENT1,
			COLOR2 = GL_COLOR_ATTACHMENT2,
			COLOR3 = GL_COLOR_ATTACHMENT3,
			COLOR4 = GL_COLOR_ATTACHMENT4,
			COLOR5 = GL_COLOR_ATTACHMENT5,
			COLOR6 = GL_COLOR_ATTACHMENT6,
			COLOR7 = GL_COLOR_ATTACHMENT7,
			COLOR8 = GL_COLOR_ATTACHMENT8,
			COLOR9 = GL_COLOR_ATTACHMENT9,
			COLOR10 = GL_COLOR_ATTACHMENT10,
			COLOR11 = GL_COLOR_ATTACHMENT11,
			COLOR12 = GL_COLOR_ATTACHMENT12,
			COLOR13 = GL_COLOR_ATTACHMENT13,
			COLOR14 = GL_COLOR_ATTACHMENT14,
			COLOR15 = GL_COLOR_ATTACHMENT15,
			DEPTH = GL_DEPTH_ATTACHMENT,
			STENCIL = GL_STENCIL_ATTACHMENT,
			DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
		};

		enum class Status
		{
			COMPLETE = GL_FRAMEBUFFER_COMPLETE,
			UNDEFINED = GL_FRAMEBUFFER_UNDEFINED,
			INCOMPLETE_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
			INCOMPLETE_MISSING_ATTACHMENT = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
			INCOMPLETE_DRAW_BUFFER = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
			INCOMPLETE_READ_BUFFER = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
			UNSUPPORTED = GL_FRAMEBUFFER_UNSUPPORTED,
			INCOMPLETE_MULTISAMPLE = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
			INCOMPLETE_LAYER_TARGETS = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
			ERROR = 0
		};

		extern void bind(ids::FrameBuffer fb, Target target = Target::DUAL);
		extern void unbind(Target target = Target::DUAL);
		extern void attach_texture(ids::Texture texture, Attachment attachment, Target target = Target::DUAL);
		extern Status status(Target target = Target::DUAL);
		extern bool is_complete(Target target = Target::DUAL);
		extern void draw_into(Attachment attachment);
		extern void draw_into(const Attachment* attachments, GLuint count);
		
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void attach_texture(ids::FrameBuffer fb, ids::Texture texture, Attachment attachment);
		extern Status status(ids::FrameBuffer fb, Target target = Target::DUAL);
		extern bool is_complete(ids::FrameBuffer fb, Target target = Target::DUAL);
		extern void draw_into(ids::FrameBuffer fb, Attachment attachment);
		extern void draw_into(ids::FrameBuffer fb, const Attachment* attachments, GLuint count);
#endif
	}

	class FrameBufferObject
	{
		raii::FrameBuffer fb;

	public:
		GLuint x, y, width, height;
		glm::vec4 clear_color = { 0.0f, 0.0f, 0.0f, 0.0f };
		framebuffers::Target target = framebuffers::Target::DUAL;

		FrameBufferObject() = default;
		FrameBufferObject(GLuint x, GLuint y, GLint width, GLuint height, glm::vec4 clear_color = { 0.0f, 0.0f, 0.0f, 1.0f }, framebuffers::Target target = framebuffers::Target::DUAL)
			: x(x), y(y), width(width), height(height), clear_color(clear_color), target(target) {}

		ids::FrameBuffer framebuffer() const { return fb; }
		void bind() const { framebuffers::bind(fb, target); }
		void attach_texture(ids::Texture texture, framebuffers::Attachment attachment);
		void attach_textures(ids::Texture* textures, const framebuffers::Attachment* attachments, GLuint count);
		void draw_into(framebuffers::Attachment attachment) const;
		void draw_into(const framebuffers::Attachment* attachments, GLuint count) const;
		void begin_render() const;
	};

	// LATER FrameBufferObjectBlock
}
