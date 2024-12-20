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

	class Texture2D
	{
		GLuint* _t = nullptr;
		GLsizei count;

	public:
		Texture2D(GLsizei count = 1);
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) noexcept;
		Texture2D& operator=(Texture2D&&) noexcept;
		~Texture2D();

		operator GLuint () const { return (*this)[0]; }
		GLuint operator[](GLsizei texture) const;
	};

	extern void bind_texture2D(GLuint texture, GLsizei slot);
	extern void unbind_texture2D(GLsizei slot);
	extern void update_bound_texture2Ds();
}
