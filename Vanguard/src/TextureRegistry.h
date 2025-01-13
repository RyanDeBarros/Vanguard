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

- 1D array
- 2D array (from spritesheets)
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
			int level;

			bool operator==(const Constructor2D&) const;
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
			int level;

			bool operator==(const ConstructorRect&) const;
		};

		struct ConstructorRectHash
		{
			size_t operator()(const ConstructorRect& c) const;
		};

		std::unordered_map<ConstructorRect, raii::Texture, ConstructorRectHash> textures_rect;
		std::unordered_map<ids::Texture, decltype(textures_rect)::const_iterator> lookup_rect;

		ids::Texture load_texture_rect(ConstructorRect&& constructor);

		struct Constructor1D
		{
			FilePath image_filepath;
			std::shared_ptr<const TextureParams> params;
			int level;

			bool operator==(const Constructor1D&) const;
		};

		struct Constructor1DHash
		{
			size_t operator()(const Constructor1D& c) const;
		};

		std::unordered_map<Constructor1D, raii::Texture, Constructor1DHash> textures_1d;
		std::unordered_map<ids::Texture, decltype(textures_1d)::const_iterator> lookup_1d;

		ids::Texture load_texture_1d(Constructor1D&& constructor);

		struct ConstructorGIF
		{
			FilePath image_filepath;
			std::shared_ptr<const TextureParams> params;
			int level;

			bool operator==(const ConstructorGIF&) const;
		};

		struct ConstructorGIFHash
		{
			size_t operator()(const ConstructorGIF& c) const;
		};

		std::unordered_map<ConstructorGIF, raii::Texture, ConstructorGIFHash> textures_gif;
		std::unordered_map<ids::Texture, decltype(textures_gif)::const_iterator> lookup_gif;

		ids::Texture load_texture_gif(ConstructorGIF&& constructor);
		ids::Texture load_texture_gif(ConstructorGIF&& constructor, std::vector<int>& delay_ms);

		struct ConstructorSpritesheet
		{
			FilePath image_filepath;
			std::shared_ptr<const TextureParams> params;
			int cell_width, cell_height;
			int level;

			bool operator==(const ConstructorSpritesheet&) const;
		};

		struct ConstructorSpritesheetHash
		{
			size_t operator()(const ConstructorSpritesheet& c) const;
		};

		std::unordered_map<ConstructorSpritesheet, raii::Texture, ConstructorSpritesheetHash> textures_spritesheet;
		std::unordered_map<ids::Texture, decltype(textures_spritesheet)::const_iterator> lookup_spritesheet;

		ids::Texture load_texture_spritesheet(ConstructorSpritesheet&& constructor);

		enum class TextureType
		{
			T2D,
			RECTANGLE,
			T1D,
			GIF_2D_ARRAY,
			SPRITESHEET_2D_ARRAY,
		};
		std::unordered_map<ids::Texture, TextureType> meta_lookup;

	public:
		ids::Texture load_texture_2d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level = 0);
		ids::Texture load_texture_rect(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level = 0);
		ids::Texture load_texture_1d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level = 0);
		ids::Texture load_texture_gif(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level = 0);
		ids::Texture load_texture_gif_and_ms(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, std::vector<int>& delay_ms, int level = 0); // TODO
		ids::Texture load_texture_spritesheet(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int cell_width, int cell_height, int level = 0); // TODO test spritesheet and gif
		// TODO load texture spritesheet with padding between cells? Send each layer individually to the Texture 2D array
		void unload_texture(ids::Texture texture);
		void unload_all();
	};
}
