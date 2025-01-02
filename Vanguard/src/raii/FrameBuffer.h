#pragma once

#include "Texture.h"

// LATER glInvalidate* functions

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
	// LATER EXT functions.

	// TODO named (direct) versions of functions everywhere

	// TODO glReadBuffer, glReadPixels, glCopyTexImage2D.

	namespace framebuffers
	{
		enum class Target
		{
			DUAL = GL_FRAMEBUFFER,
			DRAW = GL_DRAW_FRAMEBUFFER,
			READ = GL_READ_FRAMEBUFFER
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
		extern void attach_texture_layer(ids::Texture texture, Attachment attachment, Target target, GLuint layer);
		extern Status status(Target target = Target::DUAL);
		extern bool is_complete(Target target = Target::DUAL);
		extern void draw_into(Attachment attachment);
		extern void draw_into(const Attachment* attachments, GLuint count);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void attach_texture(ids::FrameBuffer fb, ids::Texture texture, Attachment attachment);
		extern void attach_texture(ids::FrameBuffer fb, ids::Texture texture, Attachment attachment, GLuint layer);
		extern Status status(ids::FrameBuffer fb, Target target = Target::DUAL);
		extern bool is_complete(ids::FrameBuffer fb, Target target = Target::DUAL);
		extern void draw_into(ids::FrameBuffer fb, Attachment attachment);
		extern void draw_into(ids::FrameBuffer fb, const Attachment* attachments, GLuint count);
#endif

		enum class ReadBuffer
		{
			FRONT_LEFT = GL_FRONT_LEFT,
			FRONT_RIGHT = GL_FRONT_RIGHT,
			BACK_LEFT = GL_BACK_LEFT,
			BACK_RIGHT = GL_BACK_RIGHT,
			FRONT = GL_FRONT,
			BACK = GL_BACK,
			LEFT = GL_LEFT,
			RIGHT = GL_RIGHT,
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
			COLOR15 = GL_COLOR_ATTACHMENT15
		};

		extern void select_read_mode(ReadBuffer mode);
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void select_read_mode(ids::FrameBuffer fb, ReadBuffer mode);
#endif

		template<tex::DataType DataType = tex::DataType::UBYTE>
		inline void read_pixels(GLuint x, GLuint y, raii::Image2D<DataType>& image)
		{
			glReadPixels(x, y, image.width(), image.height(), chpp_format(image.chpp()), (GLenum)DataType, image.pixels());
		}

		template<tex::DataType DataType = tex::DataType::UBYTE>
		inline raii::Image2D<DataType> read_pixels(GLuint x, GLuint y, GLuint width, GLuint height, CHPP chpp)
		{
			raii::Image2D<DataType> image(width, height, chpp);
			glReadPixels(x, y, width, height, chpp_format(chpp), (GLenum)DataType, image.pixels());
			return image;
		}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		template<tex::DataType DataType = tex::DataType::UBYTE>
		inline void read_n_pixels(GLuint x, GLuint y, raii::Image2D<DataType>& image)
		{
			glReadnPixels(x, y, image.width(), image.height(), chpp_format(image.chpp()), (GLenum)DataType, image.bytes(), image.pixels());
		}

		template<tex::DataType DataType = tex::DataType::UBYTE>
		inline raii::Image2D<DataType> read_n_pixels(GLuint x, GLuint y, GLuint width, GLuint height, CHPP chpp)
		{
			raii::Image2D<DataType> image(width, height, chpp);
			glReadnPixels(x, y, width, height, chpp_format(chpp), (GLenum)DataType, image.bytes(), image.pixels());
			return image;
		}
#endif

		enum class BlitMask
		{
			COLOR = GL_COLOR_BUFFER_BIT,
			DEPTH = GL_DEPTH_BUFFER_BIT,
			STENCIL = GL_STENCIL_BUFFER_BIT
		};

		// Make sure to bind the appropriate framebuffers to READ and DRAW when copying from different framebuffers.
		extern void blit(IntBounds src, IntBounds dst, BlitMask mask, MagFilter filter);
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void blit(ids::FrameBuffer src_fb, ids::FrameBuffer dst_fb, IntBounds src, IntBounds dst, BlitMask mask, MagFilter filter);
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
