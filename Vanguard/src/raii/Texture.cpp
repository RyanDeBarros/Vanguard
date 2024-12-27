#include "Texture.h"

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Errors.h"

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

void vg::_::init_bound_textures()
{
	for (int i = 0; i < 32; ++i)
		bound_texture2Ds[i] = ids::Texture();
}

void vg::bind_texture2D(vg::ids::Texture texture, GLuint slot)
{
	if (slot >= 0 && slot < 32)
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
	if (slot >= 0 && slot < 32)
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
	for (GLuint i = 0; i < 32; ++i)
	{
		glActiveTexture(i);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(bound_texture2Ds + i));
	}
}

vg::Image vg::load_image(const FilePath& filepath)
{
	Image image{};
	image.pixels = stbi_load(filepath.c_str(), &image.width, &image.height, &image.chpp, 0);
	return image;
}

void vg::load_image(Image& image, const FilePath& filepath)
{
	image.pixels = stbi_load(filepath.c_str(), &image.width, &image.height, &image.chpp, 0);
}

bool vg::save_image(const Image& image, const FilePath& filepath, ImageFormat format, JPGQuality jpg_quality)
{
	switch (format)
	{
	case ImageFormat::PNG:
		return stbi_write_png(filepath.c_str(), image.width, image.height, image.chpp, image.pixels, image.stride());
	case ImageFormat::JPG:
		return stbi_write_jpg(filepath.c_str(), image.width, image.height, image.chpp, image.pixels, int(jpg_quality));
	case ImageFormat::BMP:
		return stbi_write_bmp(filepath.c_str(), image.width, image.height, image.chpp, image.pixels);
	case ImageFormat::HDR:
	{
#pragma warning(push)
#pragma warning(disable : 6386)
		float* hdr_image = new float[image.bytes()];
		for (size_t i = 0; i < image.bytes(); ++i)
			hdr_image[i] = image.pixels[i] / 255.0f;
#pragma warning(pop)
		bool success = stbi_write_hdr(filepath.c_str(), image.width, image.height, image.chpp, hdr_image); // NOTE HDR cannot be internally stored in Image.
		delete[] hdr_image;
		return success;
	}
	case ImageFormat::TGA:
		return stbi_write_tga(filepath.c_str(), image.width, image.height, image.chpp, image.pixels);
	}
	return false;
}

void vg::delete_image(const Image& image)
{
	delete[] image.pixels;
}

static GLenum chpp_format(vg::CHPP chpp)
{
	if (chpp == 4)
		return GL_RGBA;
	else if (chpp == 3)
		return GL_RGB;
	else if (chpp == 2)
		return GL_RG;
	else if (chpp == 1)
		return GL_RED;
	else
		return 0;
}

static GLint chpp_alignment(vg::CHPP chpp)
{
	if (chpp == 4)
		return 4;
	else if (chpp == 3)
		return 1;
	else if (chpp == 2)
		return 2;
	else if (chpp == 1)
		return 1;
	else
		return 0;
}

static GLint chpp_internal_format(vg::CHPP chpp)
{
	if (chpp == 4)
		return GL_RGBA8;
	else if (chpp == 3)
		return GL_RGB8;
	else if (chpp == 2)
		return GL_RG8;
	else if (chpp == 1)
		return GL_R8;
	else
		return 0;
}

vg::ImageTexture::ImageTexture(const Image& image, Texture2DParams params)
	: _image(image)
{
	send_texture(params);
}

void vg::ImageTexture::send_texture(Texture2DParams params) const
{
	bind(0);
	params.bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, chpp_alignment(_image.chpp));
	glTexImage2D(GL_TEXTURE_2D, 0, chpp_internal_format(_image.chpp), _image.width, _image.height, 0, chpp_format(_image.chpp), GL_UNSIGNED_BYTE, _image.pixels);
}

void vg::ImageTexture::update_texture_params(Texture2DParams params) const
{
	bind(0);
	params.bind();
}

void vg::ImageTexture::update_texture() const
{
	bind(0);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _image.width, _image.height, chpp_format(_image.chpp), GL_UNSIGNED_BYTE, _image.pixels);
}

void vg::ImageTexture::update_subtexture(int x, int y, int w, int h) const
{
	if (x >= 0 && x < _image.width && y >= 0 && y < _image.height && w >= 0 && h >= 0)
	{
		if (x + w >= _image.width)
			w = _image.width - x;
		if (y + h >= _image.height)
			h = _image.height - y;
		bind(0);
		if (x == 0 && w == _image.width)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, chpp_format(_image.chpp), GL_UNSIGNED_BYTE, _image.pos(x, y));
		}
		else
		{
			int end = y + h;
			for (int r = y; r < end; ++r)
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, r, w, 1, chpp_format(_image.chpp), GL_UNSIGNED_BYTE, _image.pos(x, r));
		}
	}
}

vg::ImageTextureBlock::ImageTextureBlock(GLuint block_count)
	: _textures(block_count)
{
}

vg::ImageTextureBlock::ImageTextureBlock(const std::vector<Image>& images)
	: _images(images), _textures((GLuint)_images.size())
{
	for (GLuint i = 0; i < _textures.get_count(); ++i)
		send_texture(i);
}

vg::ImageTextureBlock::ImageTextureBlock(std::vector<Image>&& images)
	: _images(std::move(images)), _textures((GLuint)_images.size())
{
	for (GLuint i = 0; i < _textures.get_count(); ++i)
		send_texture(i);
}

void vg::ImageTextureBlock::send_texture(GLuint i, Texture2DParams params) const
{
	bind(i, 0);
	params.bind();
	const Image& image = _images[i];
	glPixelStorei(GL_UNPACK_ALIGNMENT, chpp_alignment(image.chpp));
	glTexImage2D(GL_TEXTURE_2D, 0, chpp_internal_format(image.chpp), image.width, image.height, 0, chpp_format(image.chpp), GL_UNSIGNED_BYTE, image.pixels);
}

void vg::ImageTextureBlock::update_texture_params(GLuint i, Texture2DParams params) const
{
	bind(i, 0);
	params.bind();
}

void vg::ImageTextureBlock::update_texture(GLuint i) const
{
	bind(i, 0);
	const Image& image = _images[i];
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, chpp_format(image.chpp), GL_UNSIGNED_BYTE, image.pixels);
}

void vg::ImageTextureBlock::update_subtexture(GLuint i, int x, int y, int w, int h) const
{
	const Image& image = _images[i];
	if (x >= 0 && x < image.width && y >= 0 && y < image.height && w >= 0 && h >= 0)
	{
		if (x + w >= image.width)
			w = image.width - x;
		if (y + h >= image.height)
			h = image.height - y;
		bind(i, 0);
		if (x == 0 && w == image.width)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, chpp_format(image.chpp), GL_UNSIGNED_BYTE, image.pos(x, y));
		}
		else
		{
			int end = y + h;
			for (int r = y; r < end; ++r)
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, r, w, 1, chpp_format(image.chpp), GL_UNSIGNED_BYTE, image.pos(x, r));
		}
	}
}
