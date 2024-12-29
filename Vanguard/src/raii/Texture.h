#pragma once

#include <vector>

#include "Vanguard.h"
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

	struct TextureParams
	{
		MinFilter min_filter = MinFilter::NEAREST;
		MagFilter mag_filter = MagFilter::NEAREST;
		TextureWrap wrap_s = TextureWrap::CLAMP_TO_EDGE;
		TextureWrap wrap_t = TextureWrap::CLAMP_TO_EDGE;

		void bind() const;
		bool operator==(const TextureParams& other) const = default;

		static const TextureParams LINEAR;
		static const TextureParams NEAREST;
	};

	inline const TextureParams TextureParams::LINEAR = { MinFilter::LINEAR, MagFilter::LINEAR };
	inline const TextureParams TextureParams::NEAREST = { MinFilter::NEAREST, MagFilter::NEAREST };

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

	extern void select_texture_slot(GLuint slot);

	enum class TextureTarget
	{
		_1D = GL_TEXTURE_1D,
		_2D = GL_TEXTURE_2D,
		_3D = GL_TEXTURE_3D,
		_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
		_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		RECTANGLE = GL_TEXTURE_RECTANGLE,
		CUBE_MAP = GL_TEXTURE_CUBE_MAP,
		CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
		BUFFER = GL_TEXTURE_BUFFER,
		_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
		_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	extern void bind_texture(ids::Texture texture, TextureTarget target = TextureTarget::_2D);
	extern void unbind_texture(TextureTarget target = TextureTarget::_2D);
	extern void bind_texture_to_slot(ids::Texture texture, GLuint slot);
	extern void bind_textures_to_slots(const ids::Texture* textures, GLuint first_slot, GLuint count);

	typedef GLint CHPP;

	extern void align_texture_pixels(CHPP chpp);

	namespace tex
	{
		enum class DataType
		{
			UBYTE = GL_UNSIGNED_BYTE,
			BYTE = GL_BYTE,
			USHORT = GL_UNSIGNED_SHORT,
			SHORT = GL_SHORT,
			UINT = GL_UNSIGNED_INT,
			INT = GL_INT,
			FLOAT = GL_FLOAT
		};

		enum class Target2D : unsigned char
		{
			_2D = 0,
			_1D_ARRAY = 2,
			RECTANGLE = 4
		};

		enum class Target3D : unsigned char
		{
			_3D = 0,
			_2D_ARRAY = 2
		};

		enum class CubeMapTarget
		{
			X_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			X_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			Y_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			Y_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			Z_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			Z_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			PROXY = GL_PROXY_TEXTURE_CUBE_MAP
		};

		extern void image_1d(int width, CHPP chpp, const void* pixels, int border = 0, DataType data_type = DataType::UBYTE, bool proxy = false);
		extern void image_2d(int width, int height, CHPP chpp, const void* pixels, int border = 0, DataType data_type = DataType::UBYTE, Target2D target = Target2D::_2D, bool proxy = false);
		extern void image_3d(int width, int height, int depth, CHPP chpp, const void* pixels, int border = 0, DataType data_type = DataType::UBYTE, Target3D target = Target3D::_3D, bool proxy = false);
		extern void image_cube_map(int width, int height, CHPP chpp, const void* pixels, CubeMapTarget target, int border = 0, DataType data_type = DataType::UBYTE);
		
		extern void multisample_2d(GLsizei samples, int width, int height, CHPP chpp, bool fixed, bool proxy);
		extern void multisample_3d(GLsizei samples, int width, int height, int depth, CHPP chpp, bool fixed, bool proxy);
		
		extern void subimage_1d(int xoff, int width, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE);
		extern void subimage_2d(int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE, Target2D target = Target2D::_2D);
		extern void subimage_3d(int xoff, int yoff, int zoff, int width, int height, int depth, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE, Target3D target = Target3D::_3D);
		extern void subimage_cube_map(int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, CubeMapTarget target, DataType data_type = DataType::UBYTE);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void direct_subimage_1d(ids::Texture texture, int xoff, int width, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE);
		extern void direct_subimage_2d(ids::Texture texture, int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE);
		extern void direct_subimage_3d(ids::Texture texture, int xoff, int yoff, int zoff, int width, int height, int depth, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE);
		extern void direct_subimage_cube_map(ids::Texture texture, int xoff, int yoff, int width, int height, CHPP chpp, const void* pixels, DataType data_type = DataType::UBYTE);
#endif
	}

	struct Image
	{
		unsigned char* pixels = nullptr; // TODO use VoidArray instead. provide special ref/get-pointer functions for UBYTE, FLOAT, etc.
		int width = 0;
		int height = 0;
		CHPP chpp = 0;

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

	namespace image_2d
	{
		extern void send_texture(const Image& image, ids::Texture texture, TextureParams params = {});
		extern void update_texture_params(ids::Texture texture, TextureParams params = {});
		extern void update_full_texture(const Image& image, ids::Texture texture, TextureParams params = {});
		extern void update_sub_texture(const Image& image, ids::Texture texture, int x, int y, int w, int h);
	}
}
