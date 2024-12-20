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

#include "Vendor.h"

namespace vg
{
	extern void init();
	extern void terminate();
	extern void new_frame();

	namespace _
	{
		extern bool no_gl_errors(const char* file, int line);
	}

	extern void enable_scissor_test(bool enable = true);
	extern void enable_standard_blending(bool enable = true);
	extern void set_clear_color(glm::vec4 rgba);
	extern void clear_buffer();
}
