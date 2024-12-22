#pragma once

#include <initializer_list>
#include <unordered_map>

#include "Vendor.h"
#include "utils/FilePath.h"

namespace vg
{
	enum class SubshaderType
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY
	};

	class Subshader
	{
		GLuint _s = 0;
		SubshaderType _type;

		void compile(const std::string& subshader, const char* filepath = "");

	public:
		Subshader(const FilePath& filepath, SubshaderType type);
		Subshader(const std::string& subshader, SubshaderType type);
		Subshader(const Subshader&) = delete;
		Subshader(Subshader&&) noexcept;
		Subshader& operator=(Subshader&&) noexcept;
		~Subshader();

		operator GLuint () const { return _s; }
		SubshaderType type() const { return _type; }
	};

	enum class ShaderDataType
	{
		F = GL_FLOAT,
		F2 = GL_FLOAT_VEC2,
		F3 = GL_FLOAT_VEC3,
		F4 = GL_FLOAT_VEC4,
		FM2 = GL_FLOAT_MAT2,
		FM3 = GL_FLOAT_MAT3,
		FM4 = GL_FLOAT_MAT4,
		FM2x3 = GL_FLOAT_MAT2x3,
		FM2x4 = GL_FLOAT_MAT2x4,
		FM3x2 = GL_FLOAT_MAT3x2,
		FM3x4 = GL_FLOAT_MAT3x4,
		FM4x2 = GL_FLOAT_MAT4x2,
		FM4x3 = GL_FLOAT_MAT4x3,
		I = GL_INT,
		I2 = GL_INT_VEC2,
		I3 = GL_INT_VEC3,
		I4 = GL_INT_VEC4,
		UI = GL_UNSIGNED_INT,
		UI2 = GL_UNSIGNED_INT_VEC2,
		UI3 = GL_UNSIGNED_INT_VEC3,
		UI4 = GL_UNSIGNED_INT_VEC4,
		D = GL_DOUBLE,
		D2 = GL_DOUBLE_VEC2,
		D3 = GL_DOUBLE_VEC3,
		D4 = GL_DOUBLE_VEC4,
		DM2 = GL_DOUBLE_MAT2,
		DM3 = GL_DOUBLE_MAT3,
		DM4 = GL_DOUBLE_MAT4,
		DM2x3 = GL_DOUBLE_MAT2x3,
		DM2x4 = GL_DOUBLE_MAT2x4,
		DM3x2 = GL_DOUBLE_MAT3x2,
		DM3x4 = GL_DOUBLE_MAT3x4,
		DM4x2 = GL_DOUBLE_MAT4x2,
		DM4x3 = GL_DOUBLE_MAT4x3
	};

	extern unsigned char shader_data_type_entry_count(ShaderDataType type);
	extern GLsizei shader_data_type_base_size(ShaderDataType type);
	extern bool shader_data_type_is_long(ShaderDataType type);

	struct ShaderAttribute
	{
		ShaderDataType type;
		GLint array_count;
		GLsizei offset;
	};
	typedef std::vector<ShaderAttribute> ShaderLayout;

	struct Uniform
	{
		ShaderDataType type;
		GLint array_count;
		GLint location;
	};

	class Shader
	{
		GLuint _s = 0;
		GLsizei _stride;
		ShaderLayout _layout;
		std::unordered_map<std::string, Uniform> _uniforms;

		void link(const std::initializer_list<Subshader>& subshaders);
		void load_vertex_data();
		void load_uniform_data();

	public:
		Shader(const std::initializer_list<Subshader>& subshaders);
		Shader(const FilePath& vertex, const FilePath& fragment, const FilePath& geometry = "");
		Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry = "");
		Shader(const Shader&) = delete;
		Shader(Shader&&) noexcept;
		Shader& operator=(Shader&&) noexcept;
		~Shader();

		operator GLuint () const { return _s; }
		GLint uniform_location(const std::string& name) const;
		const std::unordered_map<std::string, Uniform>& uniforms() const { return _uniforms; }
		const ShaderLayout& layout() const { return _layout; }
		GLsizei stride() const { return _stride; }
	};

	extern void bind_shader(const Shader& shader);
	extern void unbind_shader();
	extern void update_bound_shader();
}
