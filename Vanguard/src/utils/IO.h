#pragma once

#include <unordered_map>

#include <toml/toml.hpp>

#include "FilePath.h"

namespace vg
{
	namespace io
	{
		extern bool read_file(const FilePath& filepath, std::string& content);
		extern bool read_file_uc(const FilePath& filepath, unsigned char*& content, size_t& content_length);
		extern bool read_template_file(const FilePath& filepath, std::string& content, const std::unordered_map<std::string, std::string>& tmplate);
		extern bool parse_toml(const FilePath& filepath, toml::v3::parse_result& parse_result);
	}
}
