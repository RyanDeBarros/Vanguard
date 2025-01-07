#include "Texture.h"

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Errors.h"

void vg::texture_params::min_filter(Target target, MinFilter filter)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_MIN_FILTER, (GLenum)filter);
}

void vg::texture_params::mag_filter(Target target, MagFilter filter)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_MAG_FILTER, (GLenum)filter);
}

void vg::texture_params::wrap_s(Target target, TextureWrap wrap)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_S, (GLenum)wrap);
}

void vg::texture_params::wrap_t(Target target, TextureWrap wrap)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_T, (GLenum)wrap);
}

void vg::texture_params::wrap_r(Target target, TextureWrap wrap)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_R, (GLenum)wrap);
}

void vg::texture_params::swizzle_r(Target target, TextureSwizzle swizzle)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_SWIZZLE_R, (GLenum)swizzle);
}

void vg::texture_params::swizzle_g(Target target, TextureSwizzle swizzle)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_SWIZZLE_G, (GLenum)swizzle);
}

void vg::texture_params::swizzle_b(Target target, TextureSwizzle swizzle)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_SWIZZLE_B, (GLenum)swizzle);
}

void vg::texture_params::swizzle_a(Target target, TextureSwizzle swizzle)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_SWIZZLE_A, (GLenum)swizzle);
}

void vg::texture_params::swizzle_rgba(Target target, TextureSwizzle swizzle_r, TextureSwizzle swizzle_g, TextureSwizzle swizzle_b, TextureSwizzle swizzle_a)
{
	glTexParameteriv((GLenum)target, GL_TEXTURE_SWIZZLE_RGBA, glm::value_ptr(glm::ivec4{ (GLenum)swizzle_r, (GLenum)swizzle_g, (GLenum)swizzle_b, (GLenum)swizzle_a }));
}

void vg::texture_params::depth_stencil_mode(Target target, DepthStencilMode mode)
{
	glTexParameteri((GLenum)target, GL_DEPTH_STENCIL_TEXTURE_MODE, (GLenum)DepthStencilMode());
}

void vg::texture_params::base_level(Target target, int level)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_BASE_LEVEL, level);
}

void vg::texture_params::max_level(Target target, int level)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_MAX_LEVEL, level);
}

void vg::texture_params::lod_bias(Target target, float lod)
{
	glTexParameterf((GLenum)target, GL_TEXTURE_LOD_BIAS, lod);
}

void vg::texture_params::min_lod(Target target, float lod)
{
	glTexParameterf((GLenum)target, GL_TEXTURE_MIN_LOD, lod);
}

void vg::texture_params::max_lod(Target target, float lod)
{
	glTexParameterf((GLenum)target, GL_TEXTURE_MAX_LOD, lod);
}

void vg::texture_params::compare_func(Target target, TextureCompareFunc func)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_COMPARE_FUNC, (GLenum)func);
}

void vg::texture_params::compare_mode(Target target, TextureCompareMode mode)
{
	glTexParameteri((GLenum)target, GL_TEXTURE_COMPARE_MODE, (GLenum)mode);
}

void vg::texture_params::border_color(Target target, glm::ivec4 rgba)
{
	glTexParameterIiv((GLenum)target, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(rgba));
}

void vg::texture_params::border_color(Target target, glm::vec4 rgba)
{
	glTexParameterfv((GLenum)target, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(rgba));
}

void vg::texture_params::linear(Target target)
{
	min_filter(target, MinFilter::LINEAR);
	mag_filter(target, MagFilter::LINEAR);
}

void vg::texture_params::nearest(Target target)
{
	min_filter(target, MinFilter::NEAREST);
	mag_filter(target, MagFilter::NEAREST);
}

void vg::texture_params::clamp_to_edge(Target target)
{
	wrap_r(target, TextureWrap::CLAMP_TO_EDGE);
	wrap_s(target, TextureWrap::CLAMP_TO_EDGE);
	wrap_t(target, TextureWrap::CLAMP_TO_EDGE);
}

void vg::texture_params::clamp_to_border(Target target)
{
	wrap_r(target, TextureWrap::CLAMP_TO_BORDER);
	wrap_s(target, TextureWrap::CLAMP_TO_BORDER);
	wrap_t(target, TextureWrap::CLAMP_TO_BORDER);
}

void vg::texture_params::mirrored_repeat(Target target)
{
	wrap_r(target, TextureWrap::MIRRORED_REPEAT);
	wrap_s(target, TextureWrap::MIRRORED_REPEAT);
	wrap_t(target, TextureWrap::MIRRORED_REPEAT);
}

void vg::texture_params::repeat(Target target)
{
	wrap_r(target, TextureWrap::REPEAT);
	wrap_s(target, TextureWrap::REPEAT);
	wrap_t(target, TextureWrap::REPEAT);
}

void vg::texture_params::mirror_clamp_to_edge(Target target)
{
	wrap_r(target, TextureWrap::MIRROR_CLAMP_TO_EDGE);
	wrap_s(target, TextureWrap::MIRROR_CLAMP_TO_EDGE);
	wrap_t(target, TextureWrap::MIRROR_CLAMP_TO_EDGE);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)

void vg::texture_params::min_filter(ids::Texture texture, MinFilter filter)
{
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, (GLenum)filter);
}

void vg::texture_params::mag_filter(ids::Texture texture, MagFilter filter)
{
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, (GLenum)filter);
}

void vg::texture_params::wrap_s(ids::Texture texture, TextureWrap wrap)
{
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, (GLenum)wrap);
}

void vg::texture_params::wrap_t(ids::Texture texture, TextureWrap wrap)
{
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, (GLenum)wrap);
}

void vg::texture_params::wrap_r(ids::Texture texture, TextureWrap wrap)
{
	glTextureParameteri(texture, GL_TEXTURE_WRAP_R, (GLenum)wrap);
}

void vg::texture_params::swizzle_r(ids::Texture texture, TextureSwizzle swizzle)
{
	glTextureParameteri(texture, GL_TEXTURE_SWIZZLE_R, (GLenum)swizzle);
}

void vg::texture_params::swizzle_g(ids::Texture texture, TextureSwizzle swizzle)
{
	glTextureParameteri(texture, GL_TEXTURE_SWIZZLE_G, (GLenum)swizzle);
}

void vg::texture_params::swizzle_b(ids::Texture texture, TextureSwizzle swizzle)
{
	glTextureParameteri(texture, GL_TEXTURE_SWIZZLE_B, (GLenum)swizzle);
}

void vg::texture_params::swizzle_a(ids::Texture texture, TextureSwizzle swizzle)
{
	glTextureParameteri(texture, GL_TEXTURE_SWIZZLE_A, (GLenum)swizzle);
}

void vg::texture_params::swizzle_rgba(ids::Texture texture, TextureSwizzle swizzle_r, TextureSwizzle swizzle_g, TextureSwizzle swizzle_b, TextureSwizzle swizzle_a)
{
	glTextureParameteriv(texture, GL_TEXTURE_SWIZZLE_RGBA, glm::value_ptr(glm::ivec4{ (GLenum)swizzle_r, (GLenum)swizzle_g, (GLenum)swizzle_b, (GLenum)swizzle_a }));
}

void vg::texture_params::depth_stencil_mode(ids::Texture texture, DepthStencilMode mode)
{
	glTextureParameteri(texture, GL_DEPTH_STENCIL_TEXTURE_MODE, (GLenum)DepthStencilMode());
}

void vg::texture_params::base_level(ids::Texture texture, int level)
{
	glTextureParameteri(texture, GL_TEXTURE_BASE_LEVEL, level);
}

void vg::texture_params::max_level(ids::Texture texture, int level)
{
	glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, level);
}

void vg::texture_params::lod_bias(ids::Texture texture, float lod)
{
	glTextureParameterf(texture, GL_TEXTURE_LOD_BIAS, lod);
}

void vg::texture_params::min_lod(ids::Texture texture, float lod)
{
	glTextureParameterf(texture, GL_TEXTURE_MIN_LOD, lod);
}

void vg::texture_params::max_lod(ids::Texture texture, float lod)
{
	glTextureParameterf(texture, GL_TEXTURE_MAX_LOD, lod);
}

void vg::texture_params::compare_func(ids::Texture texture, TextureCompareFunc func)
{
	glTextureParameteri(texture, GL_TEXTURE_COMPARE_FUNC, (GLenum)func);
}

void vg::texture_params::compare_mode(ids::Texture texture, TextureCompareMode mode)
{
	glTextureParameteri(texture, GL_TEXTURE_COMPARE_MODE, (GLenum)mode);
}

void vg::texture_params::border_color(ids::Texture texture, glm::ivec4 rgba)
{
	glTextureParameterIiv(texture, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(rgba));
}

void vg::texture_params::border_color(ids::Texture texture, glm::vec4 rgba)
{
	glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(rgba));
}

void vg::texture_params::linear(ids::Texture texture)
{
	min_filter(texture, MinFilter::LINEAR);
	mag_filter(texture, MagFilter::LINEAR);
}

void vg::texture_params::nearest(ids::Texture texture)
{
	min_filter(texture, MinFilter::NEAREST);
	mag_filter(texture, MagFilter::NEAREST);
}

#endif

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
	else
		throw Error(ErrorCode::INVALID_TEXTURE_SLOT);
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

GLint vg::chpp_alignment(CHPP chpp)
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

GLenum vg::chpp_format(CHPP chpp)
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

GLint vg::chpp_internal_format(CHPP chpp)
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

void vg::tex::image_1d(int width, CHPP chpp, const void* pixels, Target1D target, DataType data_type, int border, int level)
{
	glTexImage1D((GLenum)target, level, chpp_internal_format(chpp), width, border, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::image_2d(int width, int height, CHPP chpp, const void* pixels, ImageTarget2D target, DataType data_type, int border, int level)
{
	glTexImage2D((GLenum)target, level, chpp_internal_format(chpp), width, height, border, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::image_3d(int width, int height, int depth, CHPP chpp, const void* pixels, ImageTarget3D target, DataType data_type, int border, int level)
{
	glTexImage3D((GLenum)target, level, chpp_internal_format(chpp), width, height, depth, border, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::multisample_2d(GLsizei samples, int width, int height, CHPP chpp, bool fixed, bool proxy)
{
	glTexImage2DMultisample(proxy ? GL_PROXY_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D_MULTISAMPLE, samples, chpp_internal_format(chpp), width, height, fixed);
}

void vg::tex::multisample_3d(GLsizei samples, int width, int height, int depth, CHPP chpp, bool fixed, bool proxy)
{
	glTexImage3DMultisample(proxy ? GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, chpp_internal_format(chpp), width, height, depth, fixed);
}

void vg::tex::subimage_1d(int xoff, int width, CHPP chpp, const void* pixels, DataType data_type, int level)
{
	glTexSubImage1D(GL_TEXTURE_1D, level, xoff, width, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_2d(int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, SubimageTarget2D target, DataType data_type, int level)
{
	glTexSubImage2D((GLenum)target, level, xoff, yoff, width, height, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_3d(int xoff, int yoff, int zoff, int width, int height, int depth, CHPP chpp, const void* pixels, SubimageTarget3D target, DataType data_type, int level)
{
	glTexSubImage3D((GLenum)target, level, xoff, yoff, zoff, width, height, depth, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::copy_image_1d(int x, int y, int width, CHPP chpp, int border, int level)
{
	glCopyTexImage1D(GL_TEXTURE_1D, level, chpp_internal_format(chpp), x, y, width, border);
}

void vg::tex::copy_image_2d(int x, int y, int width, int height, CHPP chpp, int border, int level)
{
	glCopyTexImage2D(GL_TEXTURE_2D, level, chpp_internal_format(chpp), x, y, width, height, border);
}

void vg::tex::copy_image_cube_map(int x, int y, int width, int height, CHPP chpp, CubeMapFaceTarget target, int border, int level)
{
	glCopyTexImage2D((GLenum)target, level, chpp_internal_format(chpp), x, y, width, height, border);
}

void vg::tex::copy_subimage_1d(int xoff, int x, int y, int width, int level)
{
	glCopyTexSubImage1D(GL_TEXTURE_1D, level, xoff, x, y, width);
}

void vg::tex::copy_subimage_2d(int xoff, int yoff, int x, int y, int width, int height, CopySubimageTarget2D target, int level)
{
	glCopyTexSubImage2D((GLenum)target, level, xoff, yoff, x, y, width, height);
}

void vg::tex::copy_subimage_3d(int xoff, int yoff, int zoff, int x, int y, int width, int height, SubimageTarget3D target, int level)
{
	glCopyTexSubImage3D((GLenum)target, level, xoff, yoff, zoff, x, y, width, height);
}

void vg::tex::copy_subimage_cube_map(int xoff, int yoff, int x, int y, int width, int height, CubeMapFaceTarget target, int level)
{
	glCopyTexSubImage2D((GLenum)target, level, xoff, yoff, x, y, width, height);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)

void vg::tex::subimage_1d(ids::Texture texture, int xoff, int width, CHPP chpp, const void* pixels, DataType data_type, int level)
{
	glTextureSubImage1D(texture, level, xoff, width, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_2d(ids::Texture texture, int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type, int level)
{
	glTextureSubImage2D(texture, level, xoff, yoff, width, height, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::subimage_3d(ids::Texture texture, int xoff, int yoff, int zoff, int width, int height, int depth, CHPP chpp, const void* pixels, DataType data_type, int level)
{
	glTextureSubImage3D(texture, level, xoff, yoff, zoff, width, height, depth, chpp_format(chpp), (GLenum)data_type, pixels);
}

void vg::tex::copy_subimage_1d(ids::Texture texture, int xoff, int x, int y, int width, int level)
{
	glCopyTextureSubImage1D(texture, level, xoff, x, y, width);
}

void vg::tex::copy_subimage_2d(ids::Texture texture, int xoff, int yoff, int x, int y, int width, int height, int level)
{
	glCopyTextureSubImage2D(texture, level, xoff, yoff, x, y, width, height);
}

void vg::tex::copy_subimage_3d(ids::Texture texture, int xoff, int yoff, int zoff, int x, int y, int width, int height, int level)
{
	glCopyTextureSubImage3D(texture, level, xoff, yoff, zoff, x, y, width, height);
}

#endif

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 3)
void vg::tex::copy_image_subdata(ids::Texture src, ImageSubDataTarget src_target, glm::ivec3 src_pos, ids::Texture dst, ImageSubDataTarget dst_target, glm::ivec3 dst_pos, glm::ivec3 src_size, int src_level, int dst_level)
{
	glCopyImageSubData(src, (GLenum)src_target, src_level, src_pos.x, src_pos.y, src_pos.z, dst, (GLenum)dst_target, dst_level, dst_pos.x, dst_pos.y, dst_pos.z, src_size.x, src_size.y, src_size.z);
}
#endif

void vg::tex::buffer(ids::GLBuffer buffer, CHPP chpp)
{
	glTexBuffer(GL_TEXTURE_BUFFER, chpp_internal_format(chpp), buffer);
}

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 3)
void vg::tex::buffer_range(ids::GLBuffer buffer, GLintptr offset, GLsizeiptr size, CHPP chpp)
{
	glTexBufferRange(GL_TEXTURE_BUFFER, chpp_internal_format(chpp), buffer, offset, size);
}
#endif

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)

void vg::tex::buffer(ids::Texture texture, ids::GLBuffer buffer, CHPP chpp)
{
	glTextureBuffer(texture, chpp_internal_format(chpp), buffer);
}

void vg::tex::buffer_range(ids::Texture texture, ids::GLBuffer buffer, GLintptr offset, GLsizeiptr size, CHPP chpp)
{
	glTextureBufferRange(texture, chpp_internal_format(chpp), buffer, offset, size);
}

void vg::tex::barrier()
{
	glTextureBarrier();
}

#endif

vg::Image vg::load_image(const FilePath& filepath)
{
	Image image{};
	image.pixels = stbi_load(filepath.c_str(), &image.width, &image.height, &image.chpp, 0);
	if (!image.pixels)
		throw Error(ErrorCode::FILE_IO);
	return image;
}

void vg::load_image(Image& image, const FilePath& filepath)
{
	image.pixels = stbi_load(filepath.c_str(), &image.width, &image.height, &image.chpp, 0);
	if (!image.pixels)
		throw Error(ErrorCode::FILE_IO);
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

void vg::image_2d::send_texture(int width, int height, CHPP chpp, ids::Texture texture, int border, int level)
{
	bind_texture(texture, TextureTarget::T2D);
	align_texture_pixels(chpp);
	tex::image_2d(width, height, chpp, nullptr, tex::ImageTarget2D::T2D, tex::DataType::UBYTE, border, level);
}

void vg::image_2d::send_texture(const Image& image, ids::Texture texture, int border, int level)
{
	bind_texture(texture, TextureTarget::T2D);
	align_texture_pixels(image.chpp);
	tex::image_2d(image.width, image.height, image.chpp, image.pixels, tex::ImageTarget2D::T2D, tex::DataType::UBYTE, border, level);
}

void vg::image_2d::update_full_texture(const Image& image, ids::Texture texture, int level)
{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	tex::subimage_2d(texture, 0, 0, image.width, image.height, image.chpp, image.pixels);
#else
	bind_texture(texture, TextureTarget::T2D);
	tex::subimage_2d(0, 0, image.width, image.height, image.chpp, image.pixels, tex::SubimageTarget2D::T2D, tex::DataType::UBYTE, level);
#endif
}

void vg::image_2d::update_sub_texture(const Image& image, ids::Texture texture, int x, int y, int w, int h, int level)
{
	if (x >= 0 && x < image.width && y >= 0 && y < image.height && w >= 0 && h >= 0)
	{
		if (x + w >= image.width)
			w = image.width - x;
		if (y + h >= image.height)
			h = image.height - y;
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		if (x == 0 && w == image.width)
		{
			tex::subimage_2d(texture, x, y, w, h, image.chpp, image.pos(x, y), tex::DataType::UBYTE, level);
		}
		else
		{
			int end = y + h;
			for (int r = y; r < end; ++r)
				tex::subimage_2d(texture, x, r, w, 1, image.chpp, image.pos(x, r), tex::DataType::UBYTE, level);
		}
#else
		bind_texture(texture, TextureTarget::T2D);
		if (x == 0 && w == image.width)
		{
			tex::subimage_2d(x, y, w, h, image.chpp, image.pos(x, y), tex::SubimageTarget2D::T2D, tex::DataType::UBYTE, level);
		}
		else
		{
			int end = y + h;
			for (int r = y; r < end; ++r)
				tex::subimage_2d(x, r, w, 1, image.chpp, image.pos(x, r), tex::SubimageTarget2D::T2D, tex::DataType::UBYTE, level);
		}
#endif
	}
}
