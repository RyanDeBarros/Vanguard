#include "GLConstants.h"

GLint vg::constants::max_texture_image_units;

void vg::_::query_gl_constants()
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &vg::constants::max_texture_image_units);
}
