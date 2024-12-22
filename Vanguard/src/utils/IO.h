#pragma once

#include <unordered_map>

#include <toml/toml.hpp>

#include "FilePath.h"

namespace vg
{
	namespace io
	{
		extern std::string read_file(const FilePath& filepath);
		extern void read_file_uc(const FilePath& filepath, unsigned char*& content, size_t& content_length);
		extern std::string read_template_file(const FilePath& filepath, const std::unordered_map<std::string, std::string>& tmplate);
	}
}
