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
	tex::image_2d(image.width, image.height, texture_internal_format(image.chpp, vg::TextureDataType::UBYTE), texture_format(image.chpp),
		image.pixels, tex::ImageTarget2D::T2D, TextureDataType::UBYTE, constructor.border, constructor.level);
			
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
	tex::image_2d(image.width, image.height, texture_internal_format(image.chpp, vg::TextureDataType::UBYTE), texture_format(image.chpp),
		image.pixels, tex::ImageTarget2D::RECTANGLE, TextureDataType::UBYTE, constructor.border, constructor.level);

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

size_t vg::TransientTextureRegistry::Constructor1DHash::operator()(const Constructor1D& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.border) ^ std::hash<int>{}(c.level);
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_1d(Constructor1D&& constructor)
{
	auto iter = textures_1d.find(constructor);
	if (iter != textures_1d.end())
		return iter->second;

	raii::Texture texture;
	Image image = load_image(constructor.image_filepath);
	bind_texture(texture, TextureTarget::T1D);
	align_texture_pixels(image.chpp);
	tex::image_1d(image.width * image.height, texture_internal_format(image.chpp, vg::TextureDataType::UBYTE), texture_format(image.chpp),
		image.pixels, tex::ImageTarget1D::T1D, TextureDataType::UBYTE, constructor.border, constructor.level);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	constructor.params->apply(texture);
#else
	constructor.params->apply(texture_params::Target::T1D);
#endif

	ids::Texture tid = texture;
	lookup_1d[tid] = textures_1d.insert({ std::move(constructor), std::move(texture) }).first;
	meta_lookup[tid] = TextureType::T1D;
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

vg::ids::Texture vg::TransientTextureRegistry::load_texture_1d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int border, int level)
{
	return load_texture_1d(Constructor1D{ filepath, params, border, level });
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
	{
		auto it = lookup_rect.find(texture);
		if (it != lookup_rect.end())
		{
			textures_rect.erase(it->second);
			lookup_rect.erase(it);
		}
		break;
	}
	case TextureType::T1D:
	{
		auto it = lookup_1d.find(texture);
		if (it != lookup_1d.end())
		{
			textures_1d.erase(it->second);
			lookup_1d.erase(it);
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
	textures_1d.clear();
	lookup_1d.clear();
	meta_lookup.clear();
}
