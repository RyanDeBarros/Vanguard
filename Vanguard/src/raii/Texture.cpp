#include "Texture.h"

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Errors.h"

void vg::TextureParams::bind() const
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

void vg::select_texture_slot(GLuint slot)
{
	if (slot >= 0 && slot < 32)
		glActiveTexture(GL_TEXTURE0 + slot);
	// TODO else throw error ??
}

void vg::bind_texture(vg::ids::Texture texture, TextureTarget target)
{
	glBindTexture((GLenum)target, texture);
}

void vg::unbind_texture(TextureTarget target)
{
	glBindTexture((GLenum)target, 0);
}

void vg::bind_texture_to_slot(ids::Texture texture, GLuint slot)
{
	glBindTextureUnit(slot, texture);
}

void vg::bind_textures_to_slots(const ids::Texture* textures, GLuint first_slot, GLuint count)
{
	glBindTextures(first_slot, count, (const GLuint*)textures);
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

void vg::align_texture_pixels(CHPP chpp)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, chpp_alignment(chpp));
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

static const GLenum vg_texture_targets_1d[] = { GL_TEXTURE_1D, GL_PROXY_TEXTURE_1D };
static const GLenum vg_texture_targets_2d[] = { GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_1D_ARRAY, GL_PROXY_TEXTURE_1D_ARRAY, GL_TEXTURE_RECTANGLE, GL_PROXY_TEXTURE_RECTANGLE };
static const GLenum vg_texture_targets_3d[] = { GL_TEXTURE_3D, GL_PROXY_TEXTURE_3D, GL_TEXTURE_2D_ARRAY, GL_PROXY_TEXTURE_2D_ARRAY };

void vg::tex::image_1d(int width, CHPP chpp, const void* pixels, int border, DataType data_type, bool proxy)
{
	glTexImage1D(vg_texture_targets_1d[(unsigned char)proxy], 0, chpp_internal_format(chpp), width, border, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::image_2d(int width, int height, CHPP chpp, const void* pixels, int border, DataType data_type, Target2D target, bool proxy)
{
	glTexImage2D(vg_texture_targets_2d[(unsigned char)target + (unsigned char)proxy], 0, chpp_internal_format(chpp), width, height, border, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::image_3d(int width, int height, int depth, CHPP chpp, const void* pixels, int border, DataType data_type, Target3D target, bool proxy)
{
	glTexImage3D(vg_texture_targets_3d[(unsigned char)target + (unsigned char)proxy], 0, chpp_internal_format(chpp), width, height, depth, border, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::image_cube_map(int width, int height, CHPP chpp, const void* pixels, CubeMapTarget target, int border, DataType data_type)
{
	glTexImage2D((GLenum)target, 0, chpp_internal_format(chpp), width, height, 0, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::multisample_2d(GLsizei samples, int width, int height, CHPP chpp, bool fixed, bool proxy)
{
	glTexImage2DMultisample(proxy ? GL_PROXY_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D_MULTISAMPLE, samples, chpp_internal_format(chpp), width, height, fixed);
}

void vg::tex::multisample_3d(GLsizei samples, int width, int height, int depth, CHPP chpp, bool fixed, bool proxy)
{
	glTexImage3DMultisample(proxy ? GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, chpp_internal_format(chpp), width, height, depth, fixed);
}

void vg::tex::subimage_1d(int xoff, int width, CHPP chpp, const void* pixels, DataType data_type)
{
	glTexSubImage1D(vg_texture_targets_1d[0], 0, xoff, width, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_2d(int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type, Target2D target)
{
	glTexSubImage2D(vg_texture_targets_2d[(unsigned char)target], 0, xoff, yoff, width, height, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_3d(int xoff, int yoff, int zoff, int width, int height, int depth, CHPP chpp, const void* pixels, DataType data_type, Target3D target)
{
	glTexSubImage3D(vg_texture_targets_3d[(unsigned char)target], 0, xoff, yoff, zoff, width, height, depth, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_cube_map(int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, CubeMapTarget target, DataType data_type)
{
	if (target != CubeMapTarget::PROXY)
		glTexSubImage2D((GLenum)target, 0, xoff, yoff, width, height, chpp_format(chpp), (GLenum)data_type, pixels);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)

void vg::tex::direct_subimage_1d(ids::Texture texture, int xoff, int width, CHPP chpp, const void* pixels, DataType data_type)
{
	glTextureSubImage1D(texture, 0, xoff, width, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::direct_subimage_2d(ids::Texture texture, int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type)
{
	glTextureSubImage2D(texture, 0, xoff, yoff, width, height, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::direct_subimage_3d(ids::Texture texture, int xoff, int yoff, int zoff, int width, int height, int depth, CHPP chpp, const void* pixels, DataType data_type)
{
	glTextureSubImage3D(texture, 0, xoff, yoff, zoff, width, height, depth, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::direct_subimage_cube_map(ids::Texture texture, int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type)
{
	glTextureSubImage2D(texture, 0, xoff, yoff, width, height, chpp_format(chpp), (GLenum)data_type, pixels);
}

#endif

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

void vg::image_2d::send_texture(const Image& image, ids::Texture texture, TextureParams params)
{
	bind_texture(texture);
	params.bind();
	align_texture_pixels(image.chpp);
	tex::image_2d(image.width, image.height, image.chpp, image.pixels);
}

void vg::image_2d::update_texture_params(ids::Texture texture, TextureParams params)
{
	bind_texture(texture);
	params.bind();
}

void vg::image_2d::update_full_texture(const Image& image, ids::Texture texture, TextureParams params)
{
	bind_texture(texture);
	tex::subimage_2d(0, 0, image.width, image.height, image.chpp, image.pixels);
}

void vg::image_2d::update_sub_texture(const Image& image, ids::Texture texture, int x, int y, int w, int h)
{
	if (x >= 0 && x < image.width && y >= 0 && y < image.height && w >= 0 && h >= 0)
	{
		if (x + w >= image.width)
			w = image.width - x;
		if (y + h >= image.height)
			h = image.height - y;
		bind_texture(texture);
		if (x == 0 && w == image.width)
		{
			tex::subimage_2d(x, y, w, h, image.chpp, image.pos(x, y));
		}
		else
		{
			int end = y + h;
			for (int r = y; r < end; ++r)
				tex::subimage_2d(x, r, w, 1, image.chpp, image.pos(x, r));
		}
	}
}
