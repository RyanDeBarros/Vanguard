#pragma once

#include <vector>
#include <variant>
#include <bitset>
#include <array>

#include "Vanguard.h"
#include "utils/FilePath.h"
#include "utils/VoidArray.h"
#include "utils/Meta.h"
#include "GLBuffer.h"
#include "VGMath.h"

// LATER mipmaps

namespace vg
{
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

			operator ids::Texture() const { return _t; }
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

	enum class TextureTarget
	{
		T1D = GL_TEXTURE_1D,
		T2D = GL_TEXTURE_2D,
		T1D_ARRAY = GL_TEXTURE_1D_ARRAY,
		RECTANGLE = GL_TEXTURE_RECTANGLE,
		T3D = GL_TEXTURE_3D,
		T2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		CUBE_MAP = GL_TEXTURE_CUBE_MAP,
		CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
		T2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
		T2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		BUFFER = GL_TEXTURE_BUFFER
	};

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

	enum class TextureSwizzle
	{
		RED = GL_RED,
		GREEN = GL_GREEN,
		BLUE = GL_BLUE,
		ALPHA = GL_ALPHA,
		ZERO = GL_ZERO,
		ONE = GL_ONE
	};

	enum class DepthStencilMode
	{
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
		STENCIL_INDEX = GL_STENCIL_INDEX
	};

	enum class TextureCompareMode
	{
		REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
		NONE = GL_NONE
	};

	enum class TextureCompareFunc
	{
		LEQUAL = GL_LEQUAL,
		GEQUAL = GL_GEQUAL,
		LESS = GL_LESS,
		GREATER = GL_GREATER,
		EQUAL = GL_EQUAL,
		NOTEQUAL = GL_NOTEQUAL,
		ALWAYS = GL_ALWAYS,
		NEVER = GL_NEVER
	};

	namespace texture_params
	{
		enum Target
		{
			T1D = GL_TEXTURE_1D,
			T2D = GL_TEXTURE_2D,
			T1D_ARRAY = GL_TEXTURE_1D_ARRAY,
			RECTANGLE = GL_TEXTURE_RECTANGLE,
			T3D = GL_TEXTURE_3D,
			T2D_ARRAY = GL_TEXTURE_2D_ARRAY,
			CUBE_MAP = GL_TEXTURE_CUBE_MAP,
			CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
			T2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
			T2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		};

		extern void min_filter(Target target, MinFilter filter = MinFilter::NEAREST_MIPMAP_LINEAR);
		extern void mag_filter(Target target, MagFilter filter = MagFilter::LINEAR);
		extern void wrap_s(Target target, TextureWrap wrap = TextureWrap::REPEAT);
		extern void wrap_t(Target target, TextureWrap wrap = TextureWrap::REPEAT);
		extern void wrap_r(Target target, TextureWrap wrap = TextureWrap::REPEAT);
		extern void swizzle_r(Target target, TextureSwizzle swizzle = TextureSwizzle::RED);
		extern void swizzle_g(Target target, TextureSwizzle swizzle = TextureSwizzle::GREEN);
		extern void swizzle_b(Target target, TextureSwizzle swizzle = TextureSwizzle::BLUE);
		extern void swizzle_a(Target target, TextureSwizzle swizzle = TextureSwizzle::ALPHA);
		extern void swizzle_rgba(Target target, TextureSwizzle swizzle_r = TextureSwizzle::RED, TextureSwizzle swizzle_g = TextureSwizzle::GREEN,
			TextureSwizzle swizzle_b = TextureSwizzle::BLUE, TextureSwizzle swizzle_a = TextureSwizzle::ALPHA);
		extern void depth_stencil_mode(Target target, DepthStencilMode mode = DepthStencilMode::DEPTH_COMPONENT);
		extern void base_level(Target target, int level = 0);
		extern void max_level(Target target, int level = 1000);
		extern void lod_bias(Target target, float lod = 0.0f);
		extern void min_lod(Target target, float lod = -1000);
		extern void max_lod(Target target, float lod = 1000);
		extern void compare_func(Target target, TextureCompareFunc func = TextureCompareFunc::LESS);
		extern void compare_mode(Target target, TextureCompareMode mode = TextureCompareMode::REF_TO_TEXTURE);
		extern void border_color(Target target, glm::ivec4 rgba = { 0, 0, 0, 0 });
		extern void border_color(Target target, glm::vec4 rgba = { 0, 0, 0, 0 });

		extern void linear(Target target);
		extern void nearest(Target target);
		extern void clamp_to_edge(Target target);
		extern void clamp_to_border(Target target);
		extern void mirrored_repeat(Target target);
		extern void repeat(Target target);
		extern void mirror_clamp_to_edge(Target target);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void min_filter(ids::Texture texture, MinFilter filter = MinFilter::NEAREST_MIPMAP_LINEAR);
		extern void mag_filter(ids::Texture texture, MagFilter filter = MagFilter::LINEAR);
		extern void wrap_s(ids::Texture texture, TextureWrap wrap = TextureWrap::REPEAT);
		extern void wrap_t(ids::Texture texture, TextureWrap wrap = TextureWrap::REPEAT);
		extern void wrap_r(ids::Texture texture, TextureWrap wrap = TextureWrap::REPEAT);
		extern void swizzle_r(ids::Texture texture, TextureSwizzle swizzle = TextureSwizzle::RED);
		extern void swizzle_g(ids::Texture texture, TextureSwizzle swizzle = TextureSwizzle::GREEN);
		extern void swizzle_b(ids::Texture texture, TextureSwizzle swizzle = TextureSwizzle::BLUE);
		extern void swizzle_a(ids::Texture texture, TextureSwizzle swizzle = TextureSwizzle::ALPHA);
		extern void swizzle_rgba(ids::Texture texture, TextureSwizzle swizzle_r = TextureSwizzle::RED, TextureSwizzle swizzle_g = TextureSwizzle::GREEN,
			TextureSwizzle swizzle_b = TextureSwizzle::BLUE, TextureSwizzle swizzle_a = TextureSwizzle::ALPHA);
		extern void depth_stencil_mode(ids::Texture texture, DepthStencilMode mode = DepthStencilMode::DEPTH_COMPONENT);
		extern void base_level(ids::Texture texture, int level = 0);
		extern void max_level(ids::Texture texture, int level = 1000);
		extern void lod_bias(ids::Texture texture, float lod = 0.0f);
		extern void min_lod(ids::Texture texture, float lod = -1000);
		extern void max_lod(ids::Texture texture, float lod = 1000);
		extern void compare_func(ids::Texture texture, TextureCompareFunc func = TextureCompareFunc::LESS);
		extern void compare_mode(ids::Texture texture, TextureCompareMode mode = TextureCompareMode::REF_TO_TEXTURE);
		extern void border_color(ids::Texture texture, glm::ivec4 rgba = { 0, 0, 0, 0 });
		extern void border_color(ids::Texture texture, glm::vec4 rgba = { 0, 0, 0, 0 });
#endif
	}

	class TextureParams {
		enum Index
		{
			MIN_FILTER,
			MAG_FILTER,
			WRAP_S,
			WRAP_T,
			WRAP_R,
			SWIZZLE_R,
			SWIZZLE_G,
			SWIZZLE_B,
			SWIZZLE_A,
			DEPTH_STENCIL_MODE,
			BASE_LEVEL,
			MAX_LEVEL,
			LOD_BIAS,
			MIN_LOD,
			MAX_LOD,
			COMPARE_FUNC,
			COMPARE_MODE,
			BORDER_COLOR,
			__COUNT
		};

		std::bitset<Index::__COUNT> flags;

		MinFilter min_filter = MinFilter::NEAREST_MIPMAP_LINEAR;
		MagFilter mag_filter = MagFilter::LINEAR;
		TextureWrap wrap_s = TextureWrap::REPEAT;
		TextureWrap wrap_t = TextureWrap::REPEAT;
		TextureWrap wrap_r = TextureWrap::REPEAT;
		TextureSwizzle swizzle_r = TextureSwizzle::RED;
		TextureSwizzle swizzle_g = TextureSwizzle::GREEN;
		TextureSwizzle swizzle_b = TextureSwizzle::BLUE;
		TextureSwizzle swizzle_a = TextureSwizzle::ALPHA;
		DepthStencilMode depth_stencil_mode = DepthStencilMode::DEPTH_COMPONENT;
		int base_level = 0;
		int max_level = 1000;
		float lod_bias = 0.0f;
		float min_lod = -1000.0f;
		float max_lod = 1000.0f;
		TextureCompareFunc compare_func = TextureCompareFunc::LESS;
		TextureCompareMode compare_mode = TextureCompareMode::REF_TO_TEXTURE;
		std::variant<glm::ivec4, glm::vec4> border_color = glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };

	public:
		void set_min_filter(MinFilter min_filter);
		void set_mag_filter(MagFilter mag_filter);
		void set_wrap_s(TextureWrap wrap_s);
		void set_wrap_t(TextureWrap wrap_t);
		void set_wrap_r(TextureWrap wrap_r);
		void set_swizzle_r(TextureSwizzle swizzle_r);
		void set_swizzle_g(TextureSwizzle swizzle_g);
		void set_swizzle_b(TextureSwizzle swizzle_b);
		void set_swizzle_a(TextureSwizzle swizzle_a);
		void set_swizzle(TextureSwizzle swizzle_r, TextureSwizzle swizzle_g, TextureSwizzle swizzle_b, TextureSwizzle swizzle_a);
		void set_depth_stencil_mode(DepthStencilMode depth_stencil_mode);
		void set_base_level(int base_level);
		void set_max_level(int max_level);
		void set_lod_bias(float lod_bias);
		void set_min_lod(float min_lod);
		void set_max_lod(float max_lod);
		void set_compare_func(TextureCompareFunc compare_func);
		void set_compare_mode(TextureCompareMode compare_mode);
		void set_border_color(glm::ivec4 border_color);
		void set_border_color(glm::vec4 border_color);

		void apply(texture_params::Target target) const;
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		void apply(ids::Texture texture) const;
#endif
		size_t hash() const;
		bool operator==(const TextureParams&) const = default;

		static const std::shared_ptr<const TextureParams> STANDARD_LINEAR_2D;
		static const std::shared_ptr<const TextureParams> STANDARD_LINEAR_3D;
		static const std::shared_ptr<const TextureParams> STANDARD_NEAREST_2D;
		static const std::shared_ptr<const TextureParams> STANDARD_NEAREST_3D;
	};

	inline const std::shared_ptr<const TextureParams> TextureParams::STANDARD_LINEAR_2D = std::make_shared<const TextureParams>();
	inline const std::shared_ptr<const TextureParams> TextureParams::STANDARD_LINEAR_3D = std::make_shared<const TextureParams>();
	inline const std::shared_ptr<const TextureParams> TextureParams::STANDARD_NEAREST_2D = std::make_shared<const TextureParams>();
	inline const std::shared_ptr<const TextureParams> TextureParams::STANDARD_NEAREST_3D = std::make_shared<const TextureParams>();

	namespace _
	{
		extern void set_static_texture_params();
	}

	extern void select_texture_slot(GLuint slot);
	extern void bind_texture(ids::Texture texture, TextureTarget target);
	extern void unbind_texture(TextureTarget target);
	extern void bind_texture_to_slot(ids::Texture texture, GLuint slot);
	extern void bind_textures_to_slots(const ids::Texture* textures, GLuint first_slot, GLuint count);

	enum class TextureDataType
	{
		UBYTE = GL_UNSIGNED_BYTE,
		BYTE = GL_BYTE,
		USHORT = GL_UNSIGNED_SHORT,
		SHORT = GL_SHORT,
		UINT = GL_UNSIGNED_INT,
		INT = GL_INT,
		FLOAT = GL_FLOAT
	};

	enum class TextureFormat
	{
		RGBA = GL_RGBA,
		RGB = GL_RGB,
		RG = GL_RG,
		R = GL_RED,
		BGR = GL_BGR,
		BGRA = GL_BGRA,
		R_INT = GL_RED_INTEGER,
		RG_INT = GL_RG_INTEGER,
		RGB_INT = GL_RGB_INTEGER,
		RGBA_INT = GL_RGBA_INTEGER,
		BGR_INT = GL_BGR_INTEGER,
		BGRA_INT = GL_BGRA_INTEGER,
		STENCIL_INDEX = GL_STENCIL_INDEX,
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL
	};

	enum class TextureInternalFormat
	{
		UBYTE4 = GL_RGBA8,
		UBYTE3 = GL_RGB8,
		UBYTE2 = GL_RG8,
		UBYTE1 = GL_R8,
		BYTE4 = GL_RGBA8_SNORM,
		BYTE3 = GL_RGB8_SNORM,
		BYTE2 = GL_RG8_SNORM,
		BYTE1 = GL_R8_SNORM,
		USHORT4 = GL_RGBA16,
		USHORT3 = GL_RGB16,
		USHORT2 = GL_RG16,
		USHORT1 = GL_R16,
		SHORT4 = GL_RGBA16_SNORM,
		SHORT3 = GL_RGB16_SNORM,
		SHORT2 = GL_RG16_SNORM,
		SHORT1 = GL_R16_SNORM,
		UINT4 = GL_RGBA32UI,
		UINT3 = GL_RGB32UI,
		UINT2 = GL_RG32UI,
		UINT1 = GL_R32UI,
		INT4 = GL_RGBA32I,
		INT3 = GL_RGB32I,
		INT2 = GL_RG32I,
		INT1 = GL_R32I,
		FLOAT4 = GL_RGBA32F,
		FLOAT3 = GL_RGB32F,
		FLOAT2 = GL_RG32F,
		FLOAT1 = GL_R32F
	};

	// LATER support for GL_DEPTH_COMPONENT format and other formats that aren't computed directly from CHPP.
	namespace tex
	{
		enum class ImageTarget1D
		{
			T1D = GL_TEXTURE_1D,
			T1D_PROXY = GL_PROXY_TEXTURE_1D,
		};

		enum class ImageTarget2D
		{
			T2D = GL_TEXTURE_2D,
			T2D_PROXY = GL_PROXY_TEXTURE_2D,
			T1D_ARRAY = GL_TEXTURE_1D_ARRAY,
			T1D_ARRAY_PROXY = GL_PROXY_TEXTURE_1D_ARRAY,
			RECTANGLE = GL_TEXTURE_RECTANGLE,
			RECTANGLE_PROXY = GL_PROXY_TEXTURE_RECTANGLE,
			CUBE_MAP_X_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			CUBE_MAP_X_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			CUBE_MAP_Y_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			CUBE_MAP_Y_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			CUBE_MAP_Z_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			CUBE_MAP_Z_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			CUBE_MAP_PROXY = GL_PROXY_TEXTURE_CUBE_MAP
		};

		enum class ImageTarget3D
		{
			T3D = GL_TEXTURE_3D,
			T3D_PROXY = GL_PROXY_TEXTURE_3D,
			T2D_ARRAY = GL_TEXTURE_2D_ARRAY,
			T2D_ARRAY_PROXY = GL_PROXY_TEXTURE_2D_ARRAY,
		};

		extern void image_1d(int width, TextureInternalFormat internal_format, TextureFormat format, const void* pixels, ImageTarget1D target, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		/// If target is a RECTANGLE or RECTANGLE_PROXY, level must be 0.
		extern void image_2d(int width, int height, TextureInternalFormat internal_format, TextureFormat format, const void* pixels, ImageTarget2D target, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		extern void image_3d(int width, int height, int depth, TextureInternalFormat internal_format, TextureFormat format, const void* pixels, ImageTarget3D target, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		
		extern void multisample_2d(GLsizei samples, int width, int height, TextureInternalFormat internal_format, bool fixed, bool proxy);
		extern void multisample_3d(GLsizei samples, int width, int height, int depth, TextureInternalFormat internal_format, bool fixed, bool proxy);

		enum class SubimageTarget2D
		{
			T2D = GL_TEXTURE_2D,
			T1D_ARRAY = GL_TEXTURE_1D_ARRAY,
			CUBE_MAP_X_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			CUBE_MAP_X_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			CUBE_MAP_Y_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			CUBE_MAP_Y_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			CUBE_MAP_Z_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			CUBE_MAP_Z_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		enum class SubimageTarget3D
		{
			T3D = GL_TEXTURE_3D,
			T2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		};

		extern void subimage_1d(int xoff, int width, TextureFormat format, const void* pixels, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		extern void subimage_2d(int xoff, int yoff, int width, int height, TextureFormat format, const void* pixels, SubimageTarget2D target, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		extern void subimage_3d(int xoff, int yoff, int zoff, int width, int height, int depth, TextureFormat format, const void* pixels, SubimageTarget3D target, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);

		extern void copy_image_1d(int fbx, int fby, int width, TextureInternalFormat internal_format, int level = 0);
		extern void copy_image_2d(int fbx, int fby, int width, int height, TextureInternalFormat internal_format, int level = 0);
		
		enum class CubeMapFaceTarget
		{
			X_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			X_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			Y_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			Y_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			Z_POS = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			Z_NEG = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};
		extern void copy_image_cube_map(int fbx, int fby, int width, int height, TextureInternalFormat internal_format, CubeMapFaceTarget target, int level = 0);

		enum class CopySubimageTarget2D
		{
			T2D = GL_TEXTURE_2D,
			T1D_ARRAY = GL_TEXTURE_1D_ARRAY,
			RECTANGLE = GL_TEXTURE_RECTANGLE
		};

		extern void copy_subimage_1d(int xoff, int fbx, int fby, int width, int level = 0);
		extern void copy_subimage_2d(int xoff, int yoff, int fbx, int fby, int width, int height, CopySubimageTarget2D target, int level = 0);
		extern void copy_subimage_3d(int xoff, int yoff, int zoff, int fbx, int fby, int width, int height, SubimageTarget3D target, int level = 0);
		extern void copy_subimage_cube_map(int xoff, int yoff, int fbx, int fby, int width, int height, CubeMapFaceTarget target, int level = 0);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void subimage_1d(ids::Texture texture, int xoff, int width, TextureFormat format, const void* pixels, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		extern void subimage_2d(ids::Texture texture, int xoff, int yoff, int width, int height, TextureFormat format, const void* pixels, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		extern void subimage_3d(ids::Texture texture, int xoff, int yoff, int zoff, int width, int height, int depth, TextureFormat format, const void* pixels, TextureDataType data_type = TextureDataType::UBYTE, int level = 0);
		
		extern void copy_subimage_1d(ids::Texture texture, int xoff, int fbx, int fby, int width, int level = 0);
		extern void copy_subimage_2d(ids::Texture texture, int xoff, int yoff, int fbx, int fby, int width, int height, int level = 0);
		extern void copy_subimage_3d(ids::Texture texture, int xoff, int yoff, int zoff, int fbx, int fby, int width, int height, int level = 0);
#endif

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 3)
		enum class ImageSubDataTarget
		{
			T1D = GL_TEXTURE_1D,
			T2D = GL_TEXTURE_2D,
			T1D_ARRAY = GL_TEXTURE_1D_ARRAY,
			RECTANGLE = GL_TEXTURE_RECTANGLE,
			T3D = GL_TEXTURE_3D,
			T2D_ARRAY = GL_TEXTURE_2D_ARRAY,
			CUBE_MAP = GL_TEXTURE_CUBE_MAP,
			CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
			T2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
			T2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		};

		extern void copy_image_subdata(ids::Texture src, ImageSubDataTarget src_target, glm::ivec3 src_pos, ids::Texture dst, ImageSubDataTarget dst_target, glm::ivec3 dst_pos, glm::ivec3 src_size, int src_level = 0, int dst_level = 0);
		// LATER Renderbuffer versions, where the id is renderbuffer, and target is GL_RENDERBUFFER.
#endif

		extern void buffer(ids::GLBuffer buffer, TextureInternalFormat internal_format);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 3)
		extern void buffer_range(ids::GLBuffer buffer, GLintptr offset, GLsizeiptr size, TextureInternalFormat internal_format);
#endif

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
		extern void buffer(ids::Texture texture, ids::GLBuffer buffer, TextureInternalFormat internal_format);
		extern void buffer_range(ids::Texture texture, ids::GLBuffer buffer, GLintptr offset, GLsizeiptr size, TextureInternalFormat internal_format);
		extern void barrier();
#endif
	}

	typedef GLint CHPP;
	extern GLint chpp_alignment(CHPP chpp);
	extern void align_texture_pixels(CHPP chpp);
	extern TextureFormat texture_format(CHPP chpp);
	extern TextureInternalFormat texture_internal_format(CHPP chpp, TextureDataType data_type);
	extern std::pair<CHPP, TextureDataType> extract_texture_internal_format(TextureInternalFormat internal_format);

	struct Image
	{
		unsigned char* pixels = nullptr;
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

	namespace raii
	{
		template<TextureDataType DataType = vg::TextureDataType::UBYTE>
		class Image2D
		{
			template<TextureDataType DT> struct _DTT { static_assert(false, "Unsupported texture Data Type."); };
			template<> struct _DTT<TextureDataType::UBYTE> { using type = GLubyte; };
			template<> struct _DTT<TextureDataType::BYTE> { using type = GLbyte; };
			template<> struct _DTT<TextureDataType::USHORT> { using type = GLushort; };
			template<> struct _DTT<TextureDataType::SHORT> { using type = GLshort; };
			template<> struct _DTT<TextureDataType::UINT> { using type = GLuint; };
			template<> struct _DTT<TextureDataType::INT> { using type = GLint; };
			template<> struct _DTT<TextureDataType::FLOAT> { using type = GLfloat; };

			using DT = typename _DTT<DataType>::type;

			VoidArray _pxs;
			int _w = 0;
			int _h = 0;
			CHPP _c = 0;
			
		public:
			Image2D(int width, int height, CHPP chpp)
				: _w(width), _h(height), _c(chpp), _pxs(width * height * chpp * sizeof(DT))
			{
			}
			Image2D(vg::Image&& img)
				: _w(img.width), _h(img.height), _c(img.chpp), _pxs(img.bytes(), img.pixels)
			{
			}

			int width() const { return _w; }
			int height() const { return _h; }
			CHPP chpp() const { return _c; }
			const DT* pixels() const { return (const DT*)(const void*)_pxs; }
			DT* pixels() { return (DT*)(void*)_pxs; }
			constexpr TextureDataType data_type() const { return DataType; }

			int stride() const { return _w * _c; }
			size_t number_of_pixels() const { return _w * _h * _c; }
			size_t bytes() const { return _w * _h * _c * sizeof(DT); }
			int index_offset(int x, int y) const { return x + y * _w; }
			int pixel_offset(int x, int y) const { return (x + y * _w) * _c; }
			size_t byte_offset(int x, int y) const { return (x + y * _w) * _c * sizeof(DT); }
			DT* pos(int x, int y) const { return (DT*)_pxs.at(byte_offset(x, y)); }
			void coordinates_of(int i, int& x, int& y) const { x = i % _w; y = i / _h; }
			bool same_shape_as(const Image2D& other) const { return _w == other._w && _h == other._h && _c == other._c; }
			bool same_shape_as(const vg::Image& other) const { return _w == other.width && _h == other.height && _c == other.chpp; }
		};
	}

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
	template<vg::TextureDataType DataType = vg::TextureDataType::UBYTE>
	inline raii::Image2D<DataType> load_image_2d(const FilePath& filepath) { return raii::Image2D<DataType>(std::move(load_image(filepath))); }
	extern void load_image(Image& image, const FilePath& filepath);
	extern bool save_image(const Image& image, const FilePath& filepath, ImageFormat format, JPGQuality jpg_quality = JPGQuality::HIGHEST);
	extern void delete_image(Image& image);

	struct GIFImageFrame
	{
		Image image;
		int delay_ms = 0;
	};
	extern std::vector<GIFImageFrame> load_gif(const FilePath& filepath);

	struct GIFData
	{
		Image raw_image;
		int* delay_cs = nullptr;
		int num_frames;
	};
	extern GIFData load_raw_gif(const FilePath& filepath);
	//extern bool save_gif(const std::vector<GIFImageFrame>& frames, const FilePath& filepath); // LATER use 3rd-party library to properly write to gifs.
	extern void delete_images(std::vector<GIFImageFrame>& frames);

	namespace image_2d
	{
		extern void send_texture(int width, int height, CHPP chpp, ids::Texture texture, int level = 0);
		extern void send_texture(const Image& image, ids::Texture texture, int level = 0);
		
		template<TextureDataType DataType = vg::TextureDataType::UBYTE>
		inline void send_texture(const raii::Image2D<DataType>& image, ids::Texture texture, int level = 0)
		{
			bind_texture(texture, TextureTarget::T2D);
			align_texture_pixels(image.chpp());
			tex::image_2d(image.width(), image.height(), texture_internal_format(image.chpp(), DataType), texture_format(image.chpp()), image.pixels(), tex::ImageTarget2D::T2D, DataType, level);
		}

		extern void update_full_texture(const Image& image, ids::Texture texture, int level = 0);
		
		template<TextureDataType DataType = vg::TextureDataType::UBYTE>
		inline void update_full_texture(const raii::Image2D<DataType>& image, ids::Texture texture, int level = 0)
		{
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
			tex::subimage_2d(texture, 0, 0, image.width(), image.height(), texture_format(image.chpp()), image.pixels(), DataType, level);
#else
			bind_texture(texture, TextureTarget::T2D);
			tex::subimage_2d(0, 0, image.width(), image.height(), texture_format(image.chpp()), image.pixels(), tex::Target2D::T2D, DataType, level);
#endif
		}
		
		extern void update_sub_texture(const Image& image, ids::Texture texture, int x, int y, int w, int h, int level);
		
		template<TextureDataType DataType = vg::TextureDataType::UBYTE>
		extern void update_sub_texture(const raii::Image2D<DataType>& image, ids::Texture texture, int x, int y, int w, int h, int level = 0)
		{
			if (x >= 0 && x < image.width() && y >= 0 && y < image.height() && w >= 0 && h >= 0)
			{
				if (x + w >= image.width())
					w = image.width() - x;
				if (y + h >= image.height())
					h = image.height() - y;
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
				if (x == 0 && w == image.width())
				{
					tex::subimage_2d(texture, x, y, w, h, texture_format(image.chpp()), image.pos(x, y), DataType, level);
				}
				else
				{
					int end = y + h;
					for (int r = y; r < end; ++r)
						tex::subimage_2d(texture, x, r, w, 1, texture_format(image.chpp()), image.pos(x, r), DataType, level);
				}
#else
				bind_texture(texture, TextureTarget::T2D);
				if (x == 0 && w == image.width())
				{
					tex::subimage_2d(x, y, w, h, texture_format(image.chpp()), image.pos(x, y), tex::SubimageTarget2D::T2D, DataType, level);
				}
				else
				{
					int end = y + h;
					for (int r = y; r < end; ++r)
						tex::subimage_2d(x, r, w, 1, texture_format(image.chpp()), image.pos(x, r), tex::SubimageTarget2D::T2D, DataType, level);
				}
#endif
			}
		}
	}

	namespace query
	{
		enum class TextureIProperty
		{
			WIDTH = GL_TEXTURE_WIDTH,
			HEIGHT = GL_TEXTURE_HEIGHT,
			DEPTH = GL_TEXTURE_DEPTH,
			INTERNAL_FORMAT = GL_TEXTURE_INTERNAL_FORMAT,
			RED_TYPE = GL_TEXTURE_RED_TYPE,
			GREEN_TYPE = GL_TEXTURE_GREEN_TYPE,
			BLUE_TYPE = GL_TEXTURE_BLUE_TYPE,
			ALPHA_TYPE = GL_TEXTURE_ALPHA_TYPE,
			DEPTH_TYPE = GL_TEXTURE_DEPTH_TYPE,
			RED_SIZE = GL_TEXTURE_RED_SIZE,
			GREEN_SIZE = GL_TEXTURE_GREEN_SIZE,
			BLUE_SIZE = GL_TEXTURE_BLUE_SIZE,
			ALPHA_SIZE = GL_TEXTURE_ALPHA_SIZE,
			DEPTH_SIZE = GL_TEXTURE_DEPTH_SIZE,
			COMPRESSED = GL_TEXTURE_COMPRESSED,
			COMPRESSED_IMAGE_SIZE = GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 3)
			BUFFER_OFFSET = GL_TEXTURE_BUFFER_OFFSET,
			BUFFER_SIZE = GL_TEXTURE_BUFFER_SIZE
#endif
		};

		enum class TypeInterpretation
		{
			NONE = GL_NONE,
			SIGNED_NORMALIZED = GL_SIGNED_NORMALIZED,
			UNSIGNED_NORMALIZED = GL_UNSIGNED_NORMALIZED,
			FLOAT = GL_FLOAT,
			INT = GL_INT,
			UNSIGNED_INT = GL_UNSIGNED_INT
		};

		extern int texture_iproperty(TextureTarget target, TextureIProperty property, int level = 0);
	}

	namespace cube_map
	{
		extern raii::Texture generate_from_existing(ids::Texture x_pos, ids::Texture x_neg, ids::Texture y_pos, ids::Texture y_neg, ids::Texture z_pos, ids::Texture z_neg,
			std::array<int, 6> levels = { 0, 0, 0, 0, 0, 0 });
	}
}
