#pragma once

#include <unordered_map>

#include "raii/Texture.h"

template<>
struct std::hash<vg::ids::Texture>
{
	size_t operator()(const vg::ids::Texture& t) const { return std::hash<GLuint>{}(t); }
};

/*
Types of textures :

- 1D
- 1D array
- 2D
- 2D array
- Rectangle
- 3D
- Cube map

// TODO implement loading:

- 1D
- 1D array
- 2D array
- Cube map

*/

namespace vg
{
	class TransientTextureRegistry
	{
		struct Constructor2D
		{
			FilePath image_filepath;
			std::shared_ptr<const TextureParams> params;
			int border;
			int level;

			bool operator==(const Constructor2D&) const = default;
		};

		struct Constructor2DHash
		{
			size_t operator()(const Constructor2D& c) const;
		};

		std::unordered_map<Constructor2D, raii::Texture, Constructor2DHash> textures_2d;
		std::unordered_map<ids::Texture, decltype(textures_2d)::const_iterator> lookup_2d;

		ids::Texture load_texture_2d(Constructor2D&& constructor);

		struct ConstructorRect
		{
			FilePath image_filepath;
			std::shared_ptr<const TextureParams> params;
			int border;
			int level;

			bool operator==(const ConstructorRect&) const = default;
		};

		struct ConstructorRectHash
		{
			size_t operator()(const ConstructorRect& c) const;
		};

		std::unordered_map<ConstructorRect, raii::Texture, ConstructorRectHash> textures_rect;
		std::unordered_map<ids::Texture, decltype(textures_rect)::const_iterator> lookup_rect;

		enum class TextureType
		{
			T2D,
			RECTANGLE
		};
		std::unordered_map<ids::Texture, TextureType> meta_lookup;

		ids::Texture load_texture_rect(ConstructorRect&& constructor);

	public:
		ids::Texture load_texture_2d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int border = 0, int level = 0);
		ids::Texture load_texture_rect(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int border = 0, int level = 0);
		void unload_texture(ids::Texture texture);
		void unload_all();
	};
}
