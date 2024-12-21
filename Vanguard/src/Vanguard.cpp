#include "Vanguard.h"

#include "Errors.h"
#include "Input.h"
#include "GLConstants.h"
#include "raii/Shader.h"
#include "raii/Texture.h"

// LATER use Logger
#include <iostream>

void vg::init()
{
	if (glfwInit() != GLFW_TRUE)
		throw Error(ErrorCode::GLFW_INIT);
	_::init_input();
	_::query_gl_constants();
	_::init_bound_textures();
}

void vg::terminate()
{
	glfwTerminate();
}

void vg::new_frame()
{
	glfwPollEvents();
	update_bound_shader();
	update_bound_texture2Ds();
}

bool vg::min_opengl_version_is_at_least(GLuint major, GLuint minor)
{
	return false;
}

bool vg::min_opengl_version_is_at_most(GLuint major, GLuint minor)
{
	return false;
}

// TODO in addition to printing error code, print name of error as well.

bool vg::_::no_gl_errors(const char* file, int line)
{
	bool no_err = true;
	while (GLenum error = glGetError())
	{
		std::cerr << "OpenGL[" << error << "] " << file << ":" << line << "\n";
		no_err = false;
	}
	if (!no_err)
		std::cerr.flush();
	return no_err;
}

void vg::enable_scissor_test(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void vg::enable_standard_blending(bool enable)
{
	if (enable)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);
}

void vg::set_clear_color(glm::vec4 rgba)
{
	glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void vg::clear_buffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
	VANGUARD_ASSERT_GL_OKAY
}
