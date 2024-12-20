#include "Texture.h"

#include "Errors.h"
#include "GLConstants.h"

void vg::Texture2DParams::bind() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrap_t);
}

vg::Texture2D::Texture2D(GLsizei count)
	: count(count)
{
	_t = new GLuint[count];
	glGenTextures(count, _t);
}

vg::Texture2D::Texture2D(Texture2D&& other) noexcept
	: _t(other._t), count(other.count)
{
	other._t = nullptr;
	other.count = 0;
}

vg::Texture2D& vg::Texture2D::operator=(Texture2D&& other) noexcept
{
	if (this != &other)
	{
		glDeleteTextures(count, _t);
		delete[] _t;
		_t = other._t;
		count = other.count;
		other._t = nullptr;
		other.count = 0;
	}
	return *this;
}

vg::Texture2D::~Texture2D()
{
	glDeleteTextures(count, _t);
	delete[] _t;
}

GLuint vg::Texture2D::operator[](GLsizei texture) const
{
	if (texture < count && texture >= 0)
		return _t[texture];
	throw std::out_of_range("attempted to index texture with " + std::to_string(count) + " subtextures at position " + std::to_string(texture));
}

static GLuint bound_texture2Ds[32] = {};

void vg::bind_texture2D(GLuint texture, GLsizei slot)
{
	if (slot >= 0 && slot < constants::max_texture_image_units)
	{
		if (texture != bound_texture2Ds[slot])
		{
			bound_texture2Ds[slot] = texture;
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
	}
}

void vg::unbind_texture2D(GLsizei slot)
{
	if (slot >= 0 && slot < constants::max_texture_image_units)
	{
		if (bound_texture2Ds[slot])
		{
			bound_texture2Ds[slot] = 0;
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void vg::update_bound_texture2Ds()
{
	for (size_t i = 0; i < constants::max_texture_image_units; ++i)
	{
		glActiveTexture(i);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(bound_texture2Ds + i));
	}
}
