#include "TextureRegistry.h"

size_t vg::TransientTextureRegistry::Constructor2DHash::operator()(const Constructor2D& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.border) ^ std::hash<int>{}(c.level);
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_2d(Constructor2D&& constructor)
{
	auto iter = textures_2d.find(constructor);
	if (iter != textures_2d.end())
		return iter->second;
	
	raii::Texture texture;
	Image image = load_image(constructor.image_filepath);
	bind_texture(texture, TextureTarget::T2D);
	align_texture_pixels(image.chpp);
	tex::image_2d(image.width, image.height, image.chpp, image.pixels, tex::ImageTarget2D::T2D, vg::tex::DataType::UBYTE, constructor.border, constructor.level);
			
#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	constructor.params->apply(texture);
#else
	constructor.params->apply(texture_params::Target::T2D);
#endif
	
	ids::Texture tid = texture;
	lookup_2d[tid] = textures_2d.insert({ std::move(constructor), std::move(texture) }).first;
	meta_lookup[tid] = TextureType::T2D;
	return tid;
}

size_t vg::TransientTextureRegistry::ConstructorRectHash::operator()(const ConstructorRect& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.border) ^ std::hash<int>{}(c.level);
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_rect(ConstructorRect&& constructor)
{
	auto iter = textures_rect.find(constructor);
	if (iter != textures_rect.end())
		return iter->second;

	raii::Texture texture;
	Image image = load_image(constructor.image_filepath);
	bind_texture(texture, TextureTarget::RECTANGLE);
	align_texture_pixels(image.chpp);
	tex::image_2d(image.width, image.height, image.chpp, image.pixels, tex::ImageTarget2D::RECTANGLE, vg::tex::DataType::UBYTE, constructor.border, constructor.level);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	constructor.params->apply(texture);
#else
	constructor.params->apply(texture_params::Target::RECTANGLE);
#endif

	ids::Texture tid = texture;
	lookup_rect[tid] = textures_rect.insert({ std::move(constructor), std::move(texture) }).first;
	meta_lookup[tid] = TextureType::RECTANGLE;
	return tid;
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_2d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int border, int level)
{
	return load_texture_2d(Constructor2D{ filepath, params, border, level });
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_rect(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int border, int level)
{
	return load_texture_rect(ConstructorRect{ filepath, params, border, level });
}

template<typename LookupMap, typename TexturesMap>
static void unload_from_maps(const LookupMap& lookup, const TexturesMap& textures, vg::ids::Texture texture)
{
	auto iter = lookup.find(texture);
	if (iter != lookup.end())
	{
		textures.erase(iter->second);
		lookup.erase(iter);
	}
}

void vg::TransientTextureRegistry::unload_texture(ids::Texture texture)
{
	auto iter = meta_lookup.find(texture);
	if (iter == meta_lookup.end())
		return;

	TextureType type = iter->second;
	meta_lookup.erase(iter);

	switch (type)
	{
	case TextureType::T2D:
		//unload_from_maps(lookup_2d, textures_2d, texture);
	{
		auto it = lookup_2d.find(texture);
		if (it != lookup_2d.end())
		{
			textures_2d.erase(it->second);
			lookup_2d.erase(it);
		}
		break;
	}
	case TextureType::RECTANGLE:
		//unload_from_maps(lookup_rect, textures_rect, texture);
	{
		auto it = lookup_rect.find(texture);
		if (it != lookup_rect.end())
		{
			textures_rect.erase(it->second);
			lookup_rect.erase(it);
		}
		break;
	}
	}
}

void vg::TransientTextureRegistry::unload_all()
{
	textures_2d.clear();
	lookup_2d.clear();
	textures_rect.clear();
	lookup_rect.clear();
	meta_lookup.clear();
}
