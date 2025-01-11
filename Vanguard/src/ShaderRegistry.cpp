#include "ShaderRegistry.h"

#include "utils/IO.h"

size_t vg::ShaderRegistry::ConstructorHash::operator()(const Constructor& c) const
{
	return std::hash<FilePath>{}(c.vertex) ^ std::hash<FilePath>{}(c.fragment) ^ std::hash<FilePath>{}(c.geometry);
}

size_t vg::ShaderRegistry::TemplateConstructor::tmpl_hash(const Tmpl& tmpl)
{
	size_t hash = 0;
	std::hash<std::string> hasher;
	for (const auto& [key, value] : tmpl)
	{
		size_t pair_hash = hasher(key) ^ (hasher(value) << 1);
		hash ^= pair_hash + 0x9E3779B9 + (hash << 6) + (hash >> 2);
	}
	return hash;
}

size_t vg::ShaderRegistry::TemplateConstructorHash::operator()(const TemplateConstructor& c) const
{
	return std::hash<FilePath>{}(c.vertex) ^ std::hash<FilePath>{}(c.fragment) ^ std::hash<FilePath>{}(c.geometry)
		^ TemplateConstructor::tmpl_hash(c.vtmpl) ^ TemplateConstructor::tmpl_hash(c.ftmpl) ^ TemplateConstructor::tmpl_hash(c.gtmpl);
}

vg::ids::Shader vg::ShaderRegistry::load_shader(const Constructor& constructor)
{
	auto iter = shaders.find(constructor);
	if (iter != shaders.end())
		return iter->second;
	raii::Shader shader(constructor.vertex, constructor.fragment, constructor.geometry);
	ids::Shader sid = shader;
	shaders.insert({ constructor, std::move(shader) });
	lookup[sid] = constructor;
	return sid;
}

vg::ids::Shader vg::ShaderRegistry::load_shader(const TemplateConstructor& constructor)
{
	auto iter = template_shaders.find(constructor);
	if (iter != template_shaders.end())
		return iter->second;
	raii::Shader shader(
		io::read_template_file(constructor.vertex, constructor.vtmpl),
		io::read_template_file(constructor.fragment, constructor.ftmpl),
		constructor.geometry == "" ? "" : io::read_template_file(constructor.geometry, constructor.gtmpl)
	);
	ids::Shader sid = shader;
	template_shaders.insert({ constructor, std::move(shader) });
	template_lookup[sid] = constructor;
	return sid;
}

vg::ids::Shader vg::ShaderRegistry::load_shader(const FilePath& vertex, const FilePath& fragment, const FilePath& geometry)
{
	return load_shader(Constructor{ vertex, fragment, geometry });
}

vg::ids::Shader vg::ShaderRegistry::load_shader(const FilePath& vertex, TemplateConstructor::Tmpl&& vertex_template, const FilePath& fragment,
	TemplateConstructor::Tmpl&& fragment_template, const FilePath& geometry, TemplateConstructor::Tmpl&& geometry_template)
{
	return load_shader(TemplateConstructor{ vertex, fragment, geometry, std::move(vertex_template), std::move(fragment_template), std::move(geometry_template) });
}

void vg::ShaderRegistry::unload_shader(vg::ids::Shader shader)
{
	auto iter = lookup.find(shader);
	if (iter != lookup.end())
	{
		shaders.erase(iter->second);
		lookup.erase(iter);
	}
	else
	{
		auto it = template_lookup.find(shader);
		if (it != template_lookup.end())
		{
			template_shaders.erase(it->second);
			template_lookup.erase(it);
		}
	}
}

void vg::ShaderRegistry::unload_all()
{
	shaders.clear();
	lookup.clear();
	template_shaders.clear();
	template_lookup.clear();
}

const vg::raii::Shader* vg::ShaderRegistry::ref_shader(ids::Shader shader) const
{
	auto iter = lookup.find(shader);
	if (iter != lookup.end())
		return &shaders.find(iter->second)->second;
	else
	{
		auto it = template_lookup.find(shader);
		if (it != template_lookup.end())
			return &template_shaders.find(it->second)->second;
	}
	return nullptr;
}
