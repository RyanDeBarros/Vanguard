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
#define VANGUARD_MIN_OPENGL_VERSION_MINOR 4
#endif
#define VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(major, minor) VANGUARD_MIN_OPENGL_VERSION_MAJOR > major || (VANGUARD_MIN_OPENGL_VERSION_MAJOR == major && VANGUARD_MIN_OPENGL_VERSION_MINOR >= minor)
#define VANGUARD_MIN_OPENGL_VERSION_IS_AT_MOST(major, minor) VANGUARD_MIN_OPENGL_VERSION_MAJOR < major || (VANGUARD_MIN_OPENGL_VERSION_MAJOR == major && VANGUARD_MIN_OPENGL_VERSION_MINOR <= minor)

#include "Vendor.h"

namespace vg
{
	struct InitializationConfiguration
	{
		bool standard_blending = true;
		bool scissor_test = true;
		bool vsync_on = true;
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

	extern void enable_scissor_test(bool enable = true);
	extern void enable_standard_blending(bool enable = true);
	extern void enable_vsync(bool enable = true);
	extern void set_clear_color(glm::vec4 rgba);
	extern void clear_buffer();
}
