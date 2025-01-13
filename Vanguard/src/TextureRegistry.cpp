#include "TextureRegistry.h"

bool vg::TransientTextureRegistry::Constructor2D::operator==(const Constructor2D& other) const
{
	return image_filepath == other.image_filepath && *params == *other.params && level == other.level;
}

size_t vg::TransientTextureRegistry::Constructor2DHash::operator()(const Constructor2D& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.level);
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
		image.pixels, tex::ImageTarget2D::T2D, TextureDataType::UBYTE, constructor.level);
	delete_image(image);
			
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

bool vg::TransientTextureRegistry::ConstructorRect::operator==(const ConstructorRect& other) const
{
	return image_filepath == other.image_filepath && *params == *other.params && level == other.level;
}

size_t vg::TransientTextureRegistry::ConstructorRectHash::operator()(const ConstructorRect& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.level);
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
		image.pixels, tex::ImageTarget2D::RECTANGLE, TextureDataType::UBYTE, constructor.level);
	delete_image(image);

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

bool vg::TransientTextureRegistry::Constructor1D::operator==(const Constructor1D& other) const
{
	return image_filepath == other.image_filepath && *params == *other.params && level == other.level;
}

size_t vg::TransientTextureRegistry::Constructor1DHash::operator()(const Constructor1D& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.level);
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
		image.pixels, tex::ImageTarget1D::T1D, TextureDataType::UBYTE, constructor.level);
	delete_image(image);

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

bool vg::TransientTextureRegistry::ConstructorGIF::operator==(const ConstructorGIF& other) const
{
	return image_filepath == other.image_filepath && *params == *other.params && level == other.level;
}

size_t vg::TransientTextureRegistry::ConstructorGIFHash::operator()(const ConstructorGIF& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.level);
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_gif(ConstructorGIF&& constructor)
{
	auto iter = textures_gif.find(constructor);
	if (iter != textures_gif.end())
		return iter->second;

	raii::Texture texture;
	GIFData gif = load_raw_gif(constructor.image_filepath);
	bind_texture(texture, TextureTarget::T2D_ARRAY);
	align_texture_pixels(gif.raw_image.chpp);
	tex::image_3d(gif.raw_image.width, gif.raw_image.height, gif.num_frames, texture_internal_format(gif.raw_image.chpp, TextureDataType::UBYTE),
		texture_format(gif.raw_image.chpp), gif.raw_image.pixels, tex::ImageTarget3D::T2D_ARRAY, TextureDataType::UBYTE, constructor.level);
	delete_image(gif.raw_image);
	delete[] gif.delay_cs;

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	constructor.params->apply(texture);
#else
	constructor.params->apply(texture_params::Target::T2D_ARRAY);
#endif

	ids::Texture tid = texture;
	lookup_gif[tid] = textures_gif.insert({ std::move(constructor), std::move(texture) }).first;
	meta_lookup[tid] = TextureType::GIF_2D_ARRAY;
	return tid;
}

bool vg::TransientTextureRegistry::ConstructorSpritesheet::operator==(const ConstructorSpritesheet& other) const
{
	return image_filepath == other.image_filepath && *params == *other.params && cell_width == other.cell_width && cell_height == other.cell_height && level == other.level;
}

size_t vg::TransientTextureRegistry::ConstructorSpritesheetHash::operator()(const ConstructorSpritesheet& c) const
{
	return std::hash<FilePath>{}(c.image_filepath) ^ c.params->hash() ^ std::hash<int>{}(c.cell_width) ^ std::hash<int>{}(c.cell_height) ^ std::hash<int>{}(c.level);
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_spritesheet(ConstructorSpritesheet&& constructor)
{
	auto iter = textures_spritesheet.find(constructor);
	if (iter != textures_spritesheet.end())
		return iter->second;

	raii::Texture texture;
	Image image = load_image(constructor.image_filepath);
	int cols = image.width / constructor.cell_width;
	if (cols * constructor.cell_width != image.width)
	{
		delete_image(image);
		throw Error(ErrorCode::INVALID_SPRITE_SHEET_DIMENSIONS);
	}
	int rows = image.height / constructor.cell_height;
	if (rows * constructor.cell_height!= image.height)
	{
		delete_image(image);
		throw Error(ErrorCode::INVALID_SPRITE_SHEET_DIMENSIONS);
	}

	bind_texture(texture, TextureTarget::T2D_ARRAY);
	align_texture_pixels(image.chpp);
	tex::image_3d(constructor.cell_width, constructor.cell_height, rows * cols, texture_internal_format(image.chpp, TextureDataType::UBYTE),
		texture_format(image.chpp), image.pixels, tex::ImageTarget3D::T2D_ARRAY, TextureDataType::UBYTE, constructor.level);
	delete_image(image);

#if VANGUARD_MIN_OPENGL_VERSION_IS_AT_LEAST(4, 5)
	constructor.params->apply(texture);
#else
	constructor.params->apply(texture_params::Target::T2D_ARRAY);
#endif

	ids::Texture tid = texture;
	lookup_spritesheet[tid] = textures_spritesheet.insert({ std::move(constructor), std::move(texture) }).first;
	meta_lookup[tid] = TextureType::SPRITESHEET_2D_ARRAY;
	return tid;
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_2d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level)
{
	return load_texture_2d(Constructor2D{ filepath, params, level });
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_rect(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level)
{
	return load_texture_rect(ConstructorRect{ filepath, params, level });
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_1d(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level)
{
	return load_texture_1d(Constructor1D{ filepath, params, level });
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_gif(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int level)
{
	return load_texture_gif(ConstructorGIF{ filepath, params, level });
}

vg::ids::Texture vg::TransientTextureRegistry::load_texture_spritesheet(const FilePath& filepath, const std::shared_ptr<const TextureParams>& params, int cell_width, int cell_height, int level)
{
	return load_texture_spritesheet(ConstructorSpritesheet{ filepath, params, cell_width, cell_height, level });
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
	case TextureType::GIF_2D_ARRAY:
	{
		auto it = lookup_gif.find(texture);
		if (it != lookup_gif.end())
		{
			textures_gif.erase(it->second);
			lookup_gif.erase(it);
		}
		break;
	}
	case TextureType::SPRITESHEET_2D_ARRAY:
	{
		auto it = lookup_spritesheet.find(texture);
		if (it != lookup_spritesheet.end())
		{
			textures_spritesheet.erase(it->second);
			lookup_spritesheet.erase(it);
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
	textures_gif.clear();
	lookup_gif.clear();
	textures_spritesheet.clear();
	lookup_spritesheet.clear();
	meta_lookup.clear();
}
