#pragma once

#define VANGUARD_ASSERTIONS

#ifdef VANGUARD_ASSERTIONS
#define VANGUARD_ASSERT(x) if (!(x)) __debugbreak();
#else
#define VANGUARD_ASSERT(x) ;
#endif

#define VANGUARD_GL_ERR_CHECKING

#ifdef VANGUARD_GL_ERR_CHECKING
#define VANGUARD_ASSERT_GL_OKAY VANGUARD_ASSERT(vg::_::no_gl_errors(__FILE__, __LINE__))
#else
#define VANGUARD_ASSERT_GL_OKAY ;
#endif

#ifndef VANGUARD_MIN_OPENGL_VERSION_MAJOR
#define VANGUARD_MIN_OPENGL_VERSION_MAJOR 4
#endif
#ifndef VANGUARD_MIN_OPENGL_VERSION_MINOR
#define VANGUARD_MIN_OPENGL_VERSION_MINOR 5
#endif
#define VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(major, minor) VANGUARD_MIN_OPENGL_VERSION_MAJOR > major || (VANGUARD_MIN_OPENGL_VERSION_MAJOR == major && VANGUARD_MIN_OPENGL_VERSION_MINOR >= minor)
#define VANGUARD_MIN_OPENGL_VERSION_IS_AT_MOST(major, minor) VANGUARD_MIN_OPENGL_VERSION_MAJOR < major || (VANGUARD_MIN_OPENGL_VERSION_MAJOR == major && VANGUARD_MIN_OPENGL_VERSION_MINOR <= minor)

#include "Vendor.h"
#include "VGMath.h"

namespace vg
{
	struct InitializationConfiguration
	{
		bool stbi_flip_vertically_on_load = true;
	};

	extern void init(const vg::InitializationConfiguration& config = {});
	extern void terminate();
	extern void new_frame();

	extern bool min_opengl_version_is_at_least(GLuint major, GLuint minor);
	extern bool min_opengl_version_is_at_most(GLuint major, GLuint minor);

	namespace _
	{
		extern bool no_gl_errors(const char* file, int line);
	}

	namespace enable
	{
		extern void blending(bool enable);
		extern void depth_test(bool enable);
		extern void scissor_test(bool enable);
	}

	extern void set_clear_color(glm::vec4 rgba);
	extern void clear_buffer();
	extern void set_vsync(unsigned int frames);

	extern void set_viewport(Rect<int> rect);
	extern void set_viewport(unsigned int index, Rect<float> rect);
	extern void set_viewports(unsigned int first_index, Rect<float>* rects, unsigned int count);
	extern void set_scissor(Rect<int> rect);
	extern void set_scissor(unsigned int index, Rect<int> rect);
	extern void set_scissors(unsigned int first_index, Rect<int>* rects, unsigned int count);

	enum class BlendFactor
	{
		ZERO = GL_ZERO,
		ONE = GL_ONE,
		SRC_COLOR = GL_SRC_COLOR,
		ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
		DST_COLOR = GL_DST_COLOR,
		ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
		SRC_ALPHA = GL_SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		DST_ALPHA = GL_DST_ALPHA,
		ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR = GL_CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
		SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
		SRC1_COLOR = GL_SRC1_COLOR,
		ONE_MINUS_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
		SRC1_ALPHA = GL_SRC1_ALPHA,
		ONE_MINUS_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA,
	};
	extern void set_blend_func(BlendFactor src, BlendFactor dst);

	namespace data
	{
		extern float time;
		extern float delta_time;
	}
}
