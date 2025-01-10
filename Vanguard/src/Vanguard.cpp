#include "Vanguard.h"

#include <stb/stb_image.h>

#include "Errors.h"
#include "Input.h"
#include "raii/Shader.h"
#include "raii/Texture.h"

// LATER use Logger
#include <iostream>

double vg::time = 0.0f;
double vg::delta_time = 0.0f;

void vg::init(const vg::InitializationConfiguration& config)
{
	if (glfwInit() != GLFW_TRUE)
		throw Error(ErrorCode::GLFW_INIT);
	_::init_input();
	
	if (config.stbi_flip_vertically_on_load)
		stbi_set_flip_vertically_on_load(1);
}

void vg::terminate()
{
	glfwTerminate();
}

void vg::new_frame()
{
	glfwPollEvents();
	update_bound_shader();
	double new_time = glfwGetTime();
	delta_time = new_time - time;
	time = new_time;
}

bool vg::min_opengl_version_is_at_least(GLuint major, GLuint minor)
{
	return VANGUARD_MIN_OPENGL_VERSION_MAJOR > major || (VANGUARD_MIN_OPENGL_VERSION_MAJOR == major && VANGUARD_MIN_OPENGL_VERSION_MINOR >= minor);
}

bool vg::min_opengl_version_is_at_most(GLuint major, GLuint minor)
{
	return VANGUARD_MIN_OPENGL_VERSION_MAJOR < major || (VANGUARD_MIN_OPENGL_VERSION_MAJOR == major && VANGUARD_MIN_OPENGL_VERSION_MINOR <= minor);
}

static const char* gl_error_name(GLenum error)
{
	static const char* NO_ERROR = "no error";
	static const char* INVALID_ENUM = "invalid enum";
	static const char* INVALID_VALUE = "invalid value";
	static const char* INVALID_OPERATION = "invalid operation";
	static const char* INVALID_FRAMEBUFFER_OPERATION = "invalid framebuffer operation";
	static const char* OUT_OF_MEMORY = "out of memory";
	static const char* STACK_UNDERFLOW = "stack underflow";
	static const char* STACK_OVERFLOW = "stack overflow";
	switch (error)
	{
	case GL_NO_ERROR:
		return NO_ERROR;
	case GL_INVALID_ENUM:
		return INVALID_ENUM;
	case GL_INVALID_VALUE:
		return INVALID_VALUE;
	case GL_INVALID_OPERATION:
		return INVALID_OPERATION;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return INVALID_FRAMEBUFFER_OPERATION;
	case GL_OUT_OF_MEMORY:
		return OUT_OF_MEMORY;
	case GL_STACK_UNDERFLOW:
		return STACK_UNDERFLOW;
	case GL_STACK_OVERFLOW:
		return STACK_OVERFLOW;
	default:
		return "";
	}
}

bool vg::_::no_gl_errors(const char* file, int line)
{
	bool no_err = true;
	while (GLenum error = glGetError())
	{
		const char* err = gl_error_name(error);
		if (err != "")
			std::cerr << "OpenGL[" << err << "] " << file << ":" << line << "\n";
		else
			std::cerr << "OpenGL[" << error << "] " << file << ":" << line << "\n";
		no_err = false;
	}
	if (!no_err)
		std::cerr.flush();
	return no_err;
}

void vg::enable::scissor_test(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void vg::enable::blending(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void vg::enable::depth_test(bool enable)
{
	if (enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void vg::enable::vsync(bool enable)
{
	if (enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void vg::set_clear_color(glm::vec4 rgba)
{
	glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void vg::clear_buffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void vg::set_viewport(Rect<int> rect)
{
	glViewport(rect.x, rect.y, rect.w, rect.h);
}

void vg::set_viewport(unsigned int index, Rect<float> rect)
{
	glViewportIndexedf(index, rect.x, rect.y, rect.w, rect.h);
}

void vg::set_viewports(unsigned int first_index, Rect<float>* rects, unsigned int count)
{
	glViewportArrayv(first_index, count, &rects->x);
}

void vg::set_scissor(Rect<int> rect)
{
	glScissor(rect.x, rect.y, rect.w, rect.h);
}

void vg::set_scissor(unsigned int index, Rect<int> rect)
{
	glScissorIndexed(index, rect.x, rect.y, rect.w, rect.h);
}

void vg::set_scissors(unsigned int first_index, Rect<int>* rects, unsigned int count)
{
	glScissorArrayv(first_index, count, &rects->x);
}

void vg::set_blend_func(BlendFactor src, BlendFactor dst)
{
	glBlendFunc((GLenum)src, (GLenum)dst);
}
