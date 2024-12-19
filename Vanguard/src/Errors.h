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
		WINDOW_CREATION,
		GLEW_INIT,
	};

	struct Error : public std::runtime_error
	{
		Error(ErrorCode code) : std::runtime_error("Vanguard Error (" + std::to_string((int)code) + ")") {}
		Error(const char* message = "") : std::runtime_error(message) {}
		Error(const std::string& message) : std::runtime_error(message) {}
	};
}
