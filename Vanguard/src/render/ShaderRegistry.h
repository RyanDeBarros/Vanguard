#pragma once

#include <unordered_map>

#include "raii/Shader.h"

template<>
struct std::hash<vg::ids::Shader>
{
	size_t operator()(const vg::ids::Shader& s) const { return std::hash<GLuint>{}(s); }
};

namespace vg
{
	class ShaderRegistry
	{
		struct Constructor
		{
			FilePath vertex, fragment, geometry;
			bool operator==(const Constructor&) const = default;
		};

		struct ConstructorHash
		{
			size_t operator()(const Constructor& c) const;
		};

		struct TemplateConstructor
		{
			using Tmpl = std::unordered_map<std::string, std::string>;

			static size_t tmpl_hash(const Tmpl& tmpl);

			FilePath vertex, fragment, geometry;
			Tmpl vtmpl, ftmpl, gtmpl;
			bool operator==(const TemplateConstructor&) const = default;
		};

		struct TemplateConstructorHash
		{
			size_t operator()(const TemplateConstructor& c) const;
		};

		std::unordered_map<Constructor, raii::Shader, ConstructorHash> shaders;
		std::unordered_map<ids::Shader, decltype(shaders)::const_iterator> lookup;
		std::unordered_map<TemplateConstructor, raii::Shader, TemplateConstructorHash> template_shaders;
		std::unordered_map<ids::Shader, decltype(template_shaders)::const_iterator> template_lookup;

		ids::Shader load_shader(Constructor&& constructor);
		ids::Shader load_shader(TemplateConstructor&& constructor);

	public:
		ids::Shader load_shader(const FilePath& vertex, const FilePath& fragment, const FilePath& geometry = "");
		ids::Shader load_shader(const FilePath& vertex, TemplateConstructor::Tmpl&& vertex_template, const FilePath& fragment,
			TemplateConstructor::Tmpl&& fragment_template, const FilePath& geometry = "", TemplateConstructor::Tmpl&& geometry_template = {});
		void unload_shader(ids::Shader);
		void unload_all();
		const raii::Shader* ref_shader(ids::Shader shader) const;
	};
}
