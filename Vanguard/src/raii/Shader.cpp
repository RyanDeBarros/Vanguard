#include "Shader.h"

#include "IO.h"
#include "Errors.h"

static GLenum subshader_type(vg::SubshaderType type)
{
	if (type == vg::SubshaderType::VERTEX)
		return GL_VERTEX_SHADER;
	else if (type == vg::SubshaderType::FRAGMENT)
		return GL_FRAGMENT_SHADER;
	else if (type == vg::SubshaderType::GEOMETRY)
		return GL_GEOMETRY_SHADER;
	else
		return 0;
}

void vg::Subshader::compile(const std::string& subshader, const char* filepath)
{
	_s = glCreateShader(subshader_type(_type));
	const char* c_subshader = subshader.c_str();
	glShaderSource(_s, 1, &c_subshader, nullptr);
	glCompileShader(_s);

	GLint result;
	glGetShaderiv(_s, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(_s, GL_INFO_LOG_LENGTH, &length);
		if (length)
		{
			std::string message;
			message.resize(length);
			glGetShaderInfoLog(_s, length, &length, message.data());
			glDeleteShader(_s);
			_s = 0;
			if (filepath == "")
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed - " + message);
			else
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed \"" + std::string(filepath) + "\" - " + message);
		}
		else
		{
			glDeleteShader(_s);
			_s = 0;
			if (filepath == "")
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed - no program info log provided");
			else
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed \"" + std::string(filepath) + "\" - no program info log provided");
		}
	}
}

vg::Subshader::Subshader(const FilePath& filepath, SubshaderType type)
	: _type(type)
{
	std::string subshader;
	if (vg::io::read_file(filepath, subshader))
		compile(subshader, filepath.c_str());
}

vg::Subshader::Subshader(const std::string& subshader, SubshaderType type)
	: _type(type)
{
	compile(subshader);
}

vg::Subshader::Subshader(Subshader&& other) noexcept
	: _s(other._s), _type(other._type)
{
	other._s = 0;
}

vg::Subshader& vg::Subshader::operator=(Subshader&& other) noexcept
{
	if (this != &other)
	{
		glDeleteShader(_s);
		_s = other._s;
		other._s = 0;
		_type = other._type;
	}
	return *this;
}

vg::Subshader::~Subshader()
{
	glDeleteShader(_s);
}

unsigned char vg::shader_data_type_entry_count(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::I:
	case ShaderDataType::UI:
	case ShaderDataType::F:
	case ShaderDataType::D:
		return 1;
	case ShaderDataType::I2:
	case ShaderDataType::UI2:
	case ShaderDataType::F2:
	case ShaderDataType::D2:
		return 2;
	case ShaderDataType::I3:
	case ShaderDataType::UI3:
	case ShaderDataType::F3:
	case ShaderDataType::D3:
		return 3;
	case ShaderDataType::I4:
	case ShaderDataType::UI4:
	case ShaderDataType::F4:
	case ShaderDataType::D4:
	case ShaderDataType::FM2:
	case ShaderDataType::DM2:
		return 4;
	case ShaderDataType::FM2x3:
	case ShaderDataType::FM3x2:
	case ShaderDataType::DM2x3:
	case ShaderDataType::DM3x2:
		return 6;
	case ShaderDataType::FM2x4:
	case ShaderDataType::FM4x2:
	case ShaderDataType::DM2x4:
	case ShaderDataType::DM4x2:
		return 8;
	case ShaderDataType::FM3:
	case ShaderDataType::DM3:
		return 9;
	case ShaderDataType::FM3x4:
	case ShaderDataType::FM4x3:
	case ShaderDataType::DM3x4:
	case ShaderDataType::DM4x3:
		return 12;
	case ShaderDataType::FM4:
	case ShaderDataType::DM4:
		return 16;
	default:
		return 0;
	}
}

unsigned char vg::VertexAttribute::component_count() const
{
	return array_count * shader_data_type_entry_count(type);
}

unsigned char vg::Uniform::component_count() const
{
	return array_count * shader_data_type_entry_count(type);
}

void vg::Shader::link(const std::initializer_list<Subshader>& subshaders)
{
	_s = glCreateProgram();
	for (const auto& subshader : subshaders)
		glAttachShader(_s, subshader);
	glLinkProgram(_s);
	glValidateProgram(_s);

	GLint result;
	glGetProgramiv(_s, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(_s, GL_INFO_LOG_LENGTH, &length);
		if (length)
		{
			std::string message;
			message.resize(length);
			glGetProgramInfoLog(_s, length, &length, message.data());
			glDeleteProgram(_s);
			_s = 0;
			throw Error(ErrorCode::SHADER_LINKAGE, "Shader linkage failed - " + message);
		}
		else
		{
			glDeleteProgram(_s);
			_s = 0;
			throw Error(ErrorCode::SHADER_LINKAGE, "Shader linkage failed - no program info log provided");
		}
	}
}

void vg::Shader::load_vertex_data()
{
	GLint num_attributes;
	glGetProgramiv(_s, GL_ACTIVE_ATTRIBUTES, &num_attributes);
	attribs.resize(num_attributes);
	for (GLint i = 0; i < num_attributes; ++i)
	{
		char name[256];
		GLint size;
		GLenum type;
		glGetActiveAttrib(_s, i, sizeof(name), nullptr, &size, &type, name);
		GLint location = glGetAttribLocation(_s, name);
		VertexAttribute attrib{};
		attrib.type = (ShaderDataType)type;
		attrib.array_count = size;
		attribs[location] = attrib;
	}
	for (VertexAttribute& attrib : attribs)
	{
		attrib.offset = stride;
		stride += attrib.component_count();
	}
}

void vg::Shader::load_uniform_data()
{
	GLint num_uniforms;
	glGetProgramiv(_s, GL_ACTIVE_UNIFORMS, &num_uniforms);
	for (GLint i = 0; i < num_uniforms; ++i)
	{
		char name[256];
		GLint size;
		GLenum type;
		glGetActiveUniform(_s, i, sizeof(name), nullptr, &size, &type, name);
		Uniform uniform{};
		uniform.type = (ShaderDataType)type;
		uniform.array_count = size;
		uniform.location = glGetUniformLocation(_s, name);
		uniforms.emplace(name, uniform);
	}
}

vg::Shader::Shader(const std::initializer_list<Subshader>& subshaders)
{
	link(subshaders);
	load_vertex_data();
	load_uniform_data();
}

vg::Shader::Shader(const FilePath& vertex, const FilePath& fragment, const FilePath& geometry)
{
	Subshader v(vertex, SubshaderType::VERTEX);
	Subshader f(fragment, SubshaderType::FRAGMENT);
	Subshader g(geometry, SubshaderType::GEOMETRY);
	link({ std::move(v), std::move(f), std::move(g) });
	load_vertex_data();
	load_uniform_data();
}

vg::Shader::Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry)
{
	Subshader v(vertex, SubshaderType::VERTEX);
	Subshader f(fragment, SubshaderType::FRAGMENT);
	Subshader g(geometry, SubshaderType::GEOMETRY);
	link({ std::move(v), std::move(f), std::move(g) });
	load_vertex_data();
	load_uniform_data();
}

vg::Shader::Shader(Shader&& other) noexcept
	: _s(other._s)
{
	other._s = 0;
}

vg::Shader& vg::Shader::operator=(Shader&& other) noexcept
{
	if (this != &other)
	{
		glDeleteProgram(_s);
		_s = other._s;
		other._s = 0;
	}
	return *this;
}

vg::Shader::~Shader()
{
	glDeleteProgram(_s);
}

GLint vg::Shader::uniform_location(const std::string& name) const
{
	auto iter = uniforms.find(name);
	if (iter == uniforms.end())
		return -1;
	return iter->second.location;
}

static GLuint bound_shader = 0;

void vg::bind_shader(const Shader& shader)
{
	if (shader != bound_shader)
	{
		glUseProgram(shader);
		bound_shader = shader;
	}
}

void vg::unbind_shader()
{
	if (bound_shader)
	{
		glUseProgram(0);
		bound_shader = 0;
	}
}

void vg::update_bound_shader()
{
	glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&bound_shader));
}
