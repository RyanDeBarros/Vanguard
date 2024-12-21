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

vg::raii::Texture::Texture()
{
	glGenTextures(1, (GLuint*)&_t);
}

vg::raii::Texture::Texture(Texture&& other) noexcept
	: _t(other._t)
{
	other._t = T(0);
}

vg::raii::Texture& vg::raii::Texture::operator=(Texture&& other) noexcept
{
	if (this != &other)
	{
		glDeleteTextures(1, (GLuint*)&_t);
		_t = other._t;
		other._t = T(0);
	}
	return *this;
}

vg::raii::Texture::~Texture()
{
	glDeleteTextures(1, (GLuint*)&_t);
}

vg::raii::TextureBlock::TextureBlock(GLuint count)
	: count(count)
{
	_ts = new ids::Texture[count];
	glGenTextures(count, reinterpret_cast<GLuint*>(_ts));
}

vg::raii::TextureBlock::TextureBlock(TextureBlock&& other) noexcept
	: _ts(other._ts), count(other.count)
{
	other._ts = nullptr;
	other.count = 0;
}

vg::raii::TextureBlock& vg::raii::TextureBlock::operator=(TextureBlock&& other) noexcept
{
	if (this != &other)
	{
		glDeleteTextures(count, (GLuint*)_ts);
		delete[] _ts;
		_ts = other._ts;
		other._ts = nullptr;
		count = other.count;
		other.count = 0;
	}
	return *this;
}

vg::raii::TextureBlock::~TextureBlock()
{
	glDeleteTextures(count, (GLuint*)_ts);
	delete[] _ts;
}

vg::ids::Texture vg::raii::TextureBlock::operator[](GLuint i) const
{
	if (i >= count)
		throw block_index_out_of_range(count, i);
	return _ts[i];
}

static vg::ids::Texture bound_texture2Ds[32];

void vg::_::init_textures()
{
	for (int i = 0; i < 32; ++i)
		bound_texture2Ds[i] = ids::Texture();
}

void vg::bind_texture2D(vg::ids::Texture texture, GLuint slot)
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

void vg::unbind_texture2D(GLuint slot)
{
	if (slot >= 0 && slot < constants::max_texture_image_units)
	{
		if (bound_texture2Ds[slot])
		{
			bound_texture2Ds[slot] = ids::Texture();
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
