#pragma once

#include "Macros.h"
#include "Vendor.h"
#include "utils/VGMath.h"
#include "raii/Window.h"

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
	extern void set_blend_func(BlendFactor src, BlendFactor dst);

	namespace _
	{
		extern float initial_time;
	}

	namespace data
	{
		extern float time;
		extern float delta_time;
		extern float elapsed_time();
	}

	namespace file_templates
	{
		extern std::unordered_map<std::string, std::string> num_texture_slots(const Window& window);
	}
}
