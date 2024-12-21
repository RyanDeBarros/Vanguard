#pragma once

#include "Vendor.h"

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
		extern void init_textures();
	}

	extern void bind_texture2D(ids::Texture texture, GLuint slot);
	extern void unbind_texture2D(GLuint slot);
	extern void update_bound_texture2Ds();
}
