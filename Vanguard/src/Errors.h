#pragma once

#include <stdexcept>
#include <string>

namespace vg
{
	struct CompileError : public std::exception
	{
		CompileError(const char* message = "") : std::exception(message) {}
	};

	enum class ErrorCode
	{
		VANGUARD_INIT,
		GLFW_INIT,
		GLEW_INIT,
		OPENGL_VERSION,
		WINDOW_CREATION,
		SUBSHADER_COMPILATION,
		SHADER_LINKAGE,
		BLOCK_INDEX_OUT_OF_RANGE,
		OFFSET_OUT_OF_RANGE,
	};

	struct Error : public std::runtime_error
	{
		ErrorCode code;

		Error(ErrorCode code) : std::runtime_error("Vanguard Error (" + std::to_string((int)code) + ")"), code(code) {}
		Error(ErrorCode code, const char* message) : std::runtime_error("Vanguard Error (" + std::to_string((int)code) + "): " + message), code(code) {}
		Error(ErrorCode code, const std::string& message) : std::runtime_error("Vanguard Error (" + std::to_string((int)code) + "): " + message), code(code) {}
	};

	extern Error block_index_out_of_range(size_t size, size_t index);
	extern Error offset_out_of_range(size_t size, size_t offset, size_t segment_size);
}
