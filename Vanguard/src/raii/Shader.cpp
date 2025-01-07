#include "Shader.h"

#include "utils/IO.h"
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

void vg::raii::Subshader::compile(const std::string& subshader, const char* filepath)
{
	_s = ids::Subshader(glCreateShader(subshader_type(_type)));
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
			_s = ids::Subshader(0);
			if (filepath == "")
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed - " + message);
			else
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed \"" + std::string(filepath) + "\" - " + message);
		}
		else
		{
			glDeleteShader(_s);
			_s = ids::Subshader(0);
			if (filepath == "")
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed - no program info log provided");
			else
				throw Error(ErrorCode::SUBSHADER_COMPILATION, "Subshader compilation failed \"" + std::string(filepath) + "\" - no program info log provided");
		}
	}
}

vg::raii::Subshader::Subshader(const FilePath& filepath, SubshaderType type)
	: _type(type)
{
	compile(vg::io::read_file(filepath), filepath.c_str());
}

vg::raii::Subshader::Subshader(const std::string& subshader, SubshaderType type)
	: _type(type)
{
	compile(subshader);
}

vg::raii::Subshader::Subshader(Subshader&& other) noexcept
	: _s(other._s), _type(other._type)
{
	other._s = ids::Subshader(0);
}

vg::raii::Subshader& vg::raii::Subshader::operator=(Subshader&& other) noexcept
{
	if (this != &other)
	{
		glDeleteShader(_s);
		_s = other._s;
		other._s = ids::Subshader(0);
		_type = other._type;
	}
	return *this;
}

vg::raii::Subshader::~Subshader()
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

GLsizei vg::shader_data_type_base_size(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::I:
	case ShaderDataType::I2:
	case ShaderDataType::I3:
	case ShaderDataType::I4:
		return sizeof(GLint);
	case ShaderDataType::UI:
	case ShaderDataType::UI2:
	case ShaderDataType::UI3:
	case ShaderDataType::UI4:
		return sizeof(GLuint);
	case ShaderDataType::F:
	case ShaderDataType::F2:
	case ShaderDataType::F3:
	case ShaderDataType::F4:
	case ShaderDataType::FM2:
	case ShaderDataType::FM2x3:
	case ShaderDataType::FM3x2:
	case ShaderDataType::FM2x4:
	case ShaderDataType::FM4x2:
	case ShaderDataType::FM3:
	case ShaderDataType::FM3x4:
	case ShaderDataType::FM4x3:
	case ShaderDataType::FM4:
		return sizeof(GLfloat);
	case ShaderDataType::D:
	case ShaderDataType::D2:
	case ShaderDataType::D3:
	case ShaderDataType::D4:
	case ShaderDataType::DM2:
	case ShaderDataType::DM2x3:
	case ShaderDataType::DM3x2:
	case ShaderDataType::DM2x4:
	case ShaderDataType::DM4x2:
	case ShaderDataType::DM3:
	case ShaderDataType::DM3x4:
	case ShaderDataType::DM4x3:
	case ShaderDataType::DM4:
		return sizeof(GLdouble);
	default:
		return 0;
	}
}

bool vg::shader_data_type_is_long(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::D:
	case ShaderDataType::D2:
	case ShaderDataType::D3:
	case ShaderDataType::D4:
	case ShaderDataType::DM2:
	case ShaderDataType::DM2x3:
	case ShaderDataType::DM3x2:
	case ShaderDataType::DM2x4:
	case ShaderDataType::DM4x2:
	case ShaderDataType::DM3:
	case ShaderDataType::DM3x4:
	case ShaderDataType::DM4x3:
	case ShaderDataType::DM4:
		return true;
	default:
		return false;
	}
}

void vg::raii::Shader::link(const std::initializer_list<ids::Subshader>& subshaders)
{
	_s = ids::Shader(glCreateProgram());
	for (ids::Subshader subshader : subshaders)
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
			_s = ids::Shader(0);
			throw Error(ErrorCode::SHADER_LINKAGE, "Shader linkage failed - " + message);
		}
		else
		{
			glDeleteProgram(_s);
			_s = ids::Shader(0);
			throw Error(ErrorCode::SHADER_LINKAGE, "Shader linkage failed - no program info log provided");
		}
	}
}

void vg::raii::Shader::load_vertex_data()
{
	GLint num_attributes;
	glGetProgramiv(_s, GL_ACTIVE_ATTRIBUTES, &num_attributes);
	_layout.resize(num_attributes);
	for (GLint i = 0; i < num_attributes; ++i)
	{
		char name[256];
		GLint size;
		GLenum type;
		glGetActiveAttrib(_s, i, sizeof(name), nullptr, &size, &type, name);
		GLint location = glGetAttribLocation(_s, name);
		ShaderAttribute attrib{};
		attrib.type = (ShaderDataType)type;
		attrib.array_count = size;
		_layout[location] = attrib;
	}
	_stride = 0;
	for (ShaderAttribute& attrib : _layout)
	{
		attrib.offset = _stride;
		_stride += attrib.array_count * shader_data_type_entry_count(attrib.type) * shader_data_type_base_size(attrib.type);
	}
}

void vg::raii::Shader::load_uniform_data()
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
		_uniforms.emplace(name, uniform);
	}
}

vg::raii::Shader::Shader(const std::initializer_list<ids::Subshader>& subshaders)
{
	link(subshaders);
	load_vertex_data();
	load_uniform_data();
}

vg::raii::Shader::Shader(const FilePath& vertex, const FilePath& fragment, const FilePath& geometry)
{
	Subshader v(vertex, SubshaderType::VERTEX);
	Subshader f(fragment, SubshaderType::FRAGMENT);
	if (geometry != "")
	{
		Subshader g(geometry, SubshaderType::GEOMETRY);
		link({ std::move(v), std::move(f), std::move(g) });
	}
	else
		link({ std::move(v), std::move(f) });
	load_vertex_data();
	load_uniform_data();
}

vg::raii::Shader::Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry)
{
	Subshader v(vertex, SubshaderType::VERTEX);
	Subshader f(fragment, SubshaderType::FRAGMENT);
	if (geometry != "")
	{
		Subshader g(geometry, SubshaderType::GEOMETRY);
		link({ std::move(v), std::move(f), std::move(g) });
	}
	else
		link({ std::move(v), std::move(f) });
	load_vertex_data();
	load_uniform_data();
}

vg::raii::Shader::Shader(Shader&& other) noexcept
	: _s(other._s), _stride(other._stride), _layout(std::move(other._layout)), _uniforms(std::move(other._uniforms))
{
	other._s = ids::Shader(0);
}

vg::raii::Shader& vg::raii::Shader::operator=(Shader&& other) noexcept
{
	if (this != &other)
	{
		glDeleteProgram(_s);
		_s = other._s;
		other._s = ids::Shader(0);
		_stride = other._stride;
		_layout = std::move(other._layout);
		_uniforms = std::move(other._uniforms);
	}
	return *this;
}

vg::raii::Shader::~Shader()
{
	glDeleteProgram(_s);
}

GLint vg::raii::Shader::uniform_location(const std::string& name) const
{
	auto iter = _uniforms.find(name);
	if (iter == _uniforms.end())
		return -1;
	return iter->second.location;
}

static vg::ids::Shader bound_shader = vg::ids::Shader(0);

void vg::bind_shader(vg::ids::Shader shader)
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
		bound_shader = ids::Shader(0);
	}
}

void vg::update_bound_shader()
{
	glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&bound_shader));
}

void vg::uniforms::send_1(const raii::Shader& shader, const char* uniform, float value, GLuint offset)
{
	glUniform1f(shader.uniform_location(uniform) + offset, value);
}

void vg::uniforms::send_2(const raii::Shader& shader, const char* uniform, glm::vec2 value, GLuint offset)
{
	glUniform2f(shader.uniform_location(uniform) + offset, value.x, value.y);
}

void vg::uniforms::send_3(const raii::Shader& shader, const char* uniform, glm::vec3 value, GLuint offset)
{
	glUniform3f(shader.uniform_location(uniform) + offset, value.x, value.y, value.z);
}

void vg::uniforms::send_4(const raii::Shader& shader, const char* uniform, glm::vec4 value, GLuint offset)
{
	glUniform4f(shader.uniform_location(uniform) + offset, value.x, value.y, value.z, value.w);
}

void vg::uniforms::send_1(const raii::Shader& shader, const char* uniform, int value, GLuint offset)
{
	glUniform1i(shader.uniform_location(uniform) + offset, value);
}

void vg::uniforms::send_2(const raii::Shader& shader, const char* uniform, glm::ivec2 value, GLuint offset)
{
	glUniform2i(shader.uniform_location(uniform) + offset, value.x, value.y);
}

void vg::uniforms::send_3(const raii::Shader& shader, const char* uniform, glm::ivec3 value, GLuint offset)
{
	glUniform3i(shader.uniform_location(uniform) + offset, value.x, value.y, value.z);
}

void vg::uniforms::send_4(const raii::Shader& shader, const char* uniform, glm::ivec4 value, GLuint offset)
{
	glUniform4i(shader.uniform_location(uniform) + offset, value.x, value.y, value.z, value.w);
}

void vg::uniforms::send_1(const raii::Shader& shader, const char* uniform, unsigned int value, GLuint offset)
{
	glUniform1ui(shader.uniform_location(uniform) + offset, value);
}

void vg::uniforms::send_2(const raii::Shader& shader, const char* uniform, glm::uvec2 value, GLuint offset)
{
	glUniform2ui(shader.uniform_location(uniform) + offset, value.x, value.y);
}

void vg::uniforms::send_3(const raii::Shader& shader, const char* uniform, glm::uvec3 value, GLuint offset)
{
	glUniform3ui(shader.uniform_location(uniform) + offset, value.x, value.y, value.z);
}

void vg::uniforms::send_4(const raii::Shader& shader, const char* uniform, glm::uvec4 value, GLuint offset)
{
	glUniform4ui(shader.uniform_location(uniform) + offset, value.x, value.y, value.z, value.w);
}

void vg::uniforms::send_1s(const raii::Shader& shader, const char* uniform, const float* values, GLsizei count, GLuint offset)
{
	glUniform1fv(shader.uniform_location(uniform) + offset, count, values);
}

void vg::uniforms::send_2s(const raii::Shader& shader, const char* uniform, const glm::vec2* values, GLsizei count, GLuint offset)
{
	glUniform2fv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_3s(const raii::Shader& shader, const char* uniform, const glm::vec3* values, GLsizei count, GLuint offset)
{
	glUniform3fv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_4s(const raii::Shader& shader, const char* uniform, const glm::vec4* values, GLsizei count, GLuint offset)
{
	glUniform4fv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_1s(const raii::Shader& shader, const char* uniform, const int* values, GLsizei count, GLuint offset)
{
	glUniform1iv(shader.uniform_location(uniform) + offset, count, values);
}

void vg::uniforms::send_2s(const raii::Shader& shader, const char* uniform, const glm::ivec2* values, GLsizei count, GLuint offset)
{
	glUniform2iv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_3s(const raii::Shader& shader, const char* uniform, const glm::ivec3* values, GLsizei count, GLuint offset)
{
	glUniform3iv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_4s(const raii::Shader& shader, const char* uniform, const glm::ivec4* values, GLsizei count, GLuint offset)
{
	glUniform4iv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_1s(const raii::Shader& shader, const char* uniform, const unsigned int* values, GLsizei count, GLuint offset)
{
	glUniform1uiv(shader.uniform_location(uniform) + offset, count, values);
}

void vg::uniforms::send_2s(const raii::Shader& shader, const char* uniform, const glm::uvec2* values, GLsizei count, GLuint offset)
{
	glUniform2uiv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_3s(const raii::Shader& shader, const char* uniform, const glm::uvec3* values, GLsizei count, GLuint offset)
{
	glUniform3uiv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_4s(const raii::Shader& shader, const char* uniform, const glm::uvec4* values, GLsizei count, GLuint offset)
{
	glUniform4uiv(shader.uniform_location(uniform) + offset, count, glm::value_ptr(*values));
}

void vg::uniforms::send_2x2(const raii::Shader& shader, const char* uniform, const glm::mat2& value, GLuint offset)
{
	glUniformMatrix2fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_2x3(const raii::Shader& shader, const char* uniform, const glm::mat2x3& value, GLuint offset)
{
	glUniformMatrix2x3fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_2x4(const raii::Shader& shader, const char* uniform, const glm::mat2x4& value, GLuint offset)
{
	glUniformMatrix2x4fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_3x2(const raii::Shader& shader, const char* uniform, const glm::mat3x2& value, GLuint offset)
{
	glUniformMatrix3x2fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_3x3(const raii::Shader& shader, const char* uniform, const glm::mat3& value, GLuint offset)
{
	glUniformMatrix3fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_3x4(const raii::Shader& shader, const char* uniform, const glm::mat3x4& value, GLuint offset)
{
	glUniformMatrix3x4fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_4x2(const raii::Shader& shader, const char* uniform, const glm::mat4x2& value, GLuint offset)
{
	glUniformMatrix4x2fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_4x3(const raii::Shader& shader, const char* uniform, const glm::mat4x3& value, GLuint offset)
{
	glUniformMatrix4x3fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_4x4(const raii::Shader& shader, const char* uniform, const glm::mat4& value, GLuint offset)
{
	glUniformMatrix4fv(shader.uniform_location(uniform) + offset, 1, GL_FALSE, glm::value_ptr(value));
}

void vg::uniforms::send_2x2s(const raii::Shader& shader, const char* uniform, const glm::mat2* values, GLsizei count, GLuint offset)
{
	glUniformMatrix2fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_2x3s(const raii::Shader& shader, const char* uniform, const glm::mat2x3* values, GLsizei count, GLuint offset)
{
	glUniformMatrix2x3fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_2x4s(const raii::Shader& shader, const char* uniform, const glm::mat2x4* values, GLsizei count, GLuint offset)
{
	glUniformMatrix2x4fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_3x2s(const raii::Shader& shader, const char* uniform, const glm::mat3x2* values, GLsizei count, GLuint offset)
{
	glUniformMatrix3x2fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_3x3s(const raii::Shader& shader, const char* uniform, const glm::mat3* values, GLsizei count, GLuint offset)
{
	glUniformMatrix3fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_3x4s(const raii::Shader& shader, const char* uniform, const glm::mat3x4* values, GLsizei count, GLuint offset)
{
	glUniformMatrix3x4fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_4x2s(const raii::Shader& shader, const char* uniform, const glm::mat4x2* values, GLsizei count, GLuint offset)
{
	glUniformMatrix4x2fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_4x3s(const raii::Shader& shader, const char* uniform, const glm::mat4x3* values, GLsizei count, GLuint offset)
{
	glUniformMatrix4x3fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}

void vg::uniforms::send_4x4s(const raii::Shader& shader, const char* uniform, const glm::mat4* values, GLsizei count, GLuint offset)
{
	glUniformMatrix4fv(shader.uniform_location(uniform) + offset, count, GL_FALSE, glm::value_ptr(*values));
}
