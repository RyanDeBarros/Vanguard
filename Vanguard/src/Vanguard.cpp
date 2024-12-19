#include "Vanguard.h"

// TODO use Logger
#include <iostream>

bool vg::_::no_gl_errors(const char* file, int line)
{
	bool no_err = true;
	//while (GLenum error = glGetError())
	//{
		//std::cerr << "OpenGL[" << error << "] " << file << ":" << line << "\n";
		//no_err = false;
	//}
	if (GLenum error = glGetError())
	{
		std::cerr << "OpenGL[" << error << "] " << file << ":" << line << "\n";
		no_err = false;
	}
	if (GLenum error = glGetError())
	{
		std::cerr << "OpenGL[" << error << "] " << file << ":" << line << "\n";
		no_err = false;
	}
	if (GLenum error = glGetError())
	{
		std::cerr << "OpenGL[" << error << "] " << file << ":" << line << "\n";
		no_err = false;
	}
	if (GLenum error = glGetError())
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
