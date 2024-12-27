#pragma once

#include <vector>

#include "Vendor.h"
#include "utils/FilePath.h"

namespace vg
{
	enum class MinFilter
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class MagFilter
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum class TextureWrap : GLint
	{
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		REPEAT = GL_REPEAT,
		MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
	};

	struct Texture2DParams
	{
		MinFilter min_filter = MinFilter::NEAREST;
		MagFilter mag_filter = MagFilter::NEAREST;
		TextureWrap wrap_s = TextureWrap::CLAMP_TO_EDGE;
		TextureWrap wrap_t = TextureWrap::CLAMP_TO_EDGE;

		void bind() const;
		bool operator==(const Texture2DParams& other) const = default;

		static const Texture2DParams LINEAR;
	};

	inline const Texture2DParams Texture2DParams::LINEAR = { MinFilter::LINEAR, MagFilter::LINEAR };

	namespace ids
	{
		class Texture
		{
			GLuint id;

		public:
			explicit Texture(GLuint id = 0) : id(id) {}
			operator GLuint () const { return id; }
		};
	}

	namespace raii
	{
		class Texture
		{
			using T = ids::Texture;
			T _t = T(0);

		public:
			Texture();
			Texture(const Texture&) = delete;
			Texture(Texture&&) noexcept;
			Texture& operator=(Texture&&) noexcept;
			~Texture();

			operator ids::Texture () const { return _t; }
		};

		class TextureBlock
		{
			using T = ids::Texture;
			T* _ts = nullptr;
			GLuint count;

		public:
			TextureBlock(GLuint count);
			TextureBlock(const TextureBlock&) = delete;
			TextureBlock(TextureBlock&&) noexcept;
			TextureBlock& operator=(TextureBlock&&) noexcept;
			~TextureBlock();

			ids::Texture operator[](GLuint i) const;
			GLuint get_count() const { return count; }
		};
	}

	namespace _
	{
		extern void init_bound_textures();
	}

	extern void bind_texture2D(ids::Texture texture, GLuint slot);
	extern void unbind_texture2D(GLuint slot);
	extern void update_bound_texture2Ds();

	typedef GLint CHPP;

	struct Image
	{
		unsigned char* pixels;
		int width;
		int height;
		CHPP chpp;

		void pxnew() { pixels = new unsigned char[width * height * chpp]; }
		int stride() const { return width * chpp; }
		int bytes() const { return width * height * chpp; }
		int index_offset(int x, int y) const { return x + y * width; }
		int byte_offset(int x, int y) const { return (x + y * width) * chpp; }
		unsigned char* pos(int x, int y) const { return pixels + (x + y * width) * chpp; }
		void coordinates_of(int i, int& x, int& y) const { x = i % width; y = i / height; }
		bool same_shape_as(const Image& other) const { return width == other.width && height == other.height && chpp == other.chpp; }
	};

	enum class ImageFormat
	{
		PNG,
		JPG,
		BMP,
		HDR,
		TGA
	};

	enum class JPGQuality
	{
		HIGHEST = 100,
		VERY_HIGH = 90,
		HIGH = 75,
		MEDIUM = 50,
		LOW = 25,
		VERY_LOW = 10,
		LOWEST = 1
	};

	extern Image load_image(const FilePath& filepath);
	extern void load_image(Image& image, const FilePath& filepath);
	extern bool save_image(const Image& image, const FilePath& filepath, ImageFormat format, JPGQuality jpg_quality = JPGQuality::HIGHEST);
	extern void delete_image(const Image& image);

	class ImageTexture
	{
		Image _image;
		raii::Texture _texture;

	public:
		ImageTexture() = default;
		ImageTexture(const Image& image, Texture2DParams params = {});

		void bind(GLuint slot) const { bind_texture2D(_texture, slot); }
		ids::Texture texture() const { return _texture; }
		const Image& image() const { return _image; }
		Image& image() { return _image; }

		void send_texture(Texture2DParams params = {}) const;
		void update_texture_params(Texture2DParams params = {}) const;
		void update_texture() const;
		void update_subtexture(int x, int y, int w, int h) const;
	};

	class ImageTextureBlock
	{
		std::vector<Image> _images;
		raii::TextureBlock _textures;

	public:
		ImageTextureBlock(GLuint block_count);
		ImageTextureBlock(const std::vector<Image>& images);
		ImageTextureBlock(std::vector<Image>&& images);

		void bind(GLuint i, GLuint slot) const { bind_texture2D(_textures[i], slot); }
		ids::Texture texture(GLuint i) const { return _textures[i]; }
		const Image& image(GLuint i) const { return _images[i]; }
		Image& image(GLuint i) { return _images[i]; }
		GLuint get_count() const { return _textures.get_count(); }

		void send_texture(GLuint i, Texture2DParams params = {}) const;
		void update_texture_params(GLuint i, Texture2DParams params = {}) const;
		void update_texture(GLuint i) const;
		void update_subtexture(GLuint i, int x, int y, int w, int h) const;
	};
}
