#include "IO.h"

// LATER use Logger instead
#include <iostream>

bool vg::io::read_file(const FilePath& filepath, std::string& content)
{
	std::ifstream file(filepath.c_str(), std::ios_base::in);
	if (file)
	{
		std::ostringstream oss;
		oss << file.rdbuf();
		content = oss.str();
		return true;
	}
	std::cerr << "Could not open \"" << filepath.c_str() << "\" for reading" << std::endl;
	return false;
}

bool vg::io::read_file_uc(const FilePath& filepath, unsigned char*& content, size_t& content_length)
{
	FILE* file;
	if (fopen_s(&file, filepath.c_str(), "r") == 0 && file)
	{
		fseek(file, 0, SEEK_END);
		content_length = ftell(file);
		fseek(file, 0, SEEK_SET);
		content = new unsigned char[content_length];
		fread(content, content_length, 1, file);
		fclose(file);
		return true;
	}
	return false;
}

bool vg::io::read_template_file(const FilePath& filepath, std::string& content, const std::unordered_map<std::string, std::string>& tmplate)
{
	if (!read_file(filepath, content))
		return false;
	for (const auto& [placeholder, value] : tmplate)
	{
		size_t pos = 0;
		while ((pos = content.find(placeholder, pos)) != std::string::npos)
		{
			content.replace(pos, placeholder.length(), value);
			pos += value.length();
		}
	}
	return true;
}

bool vg::io::parse_toml(const FilePath& filepath, toml::v3::parse_result& parse_result)
{
	try
	{
		parse_result = toml::parse_file(filepath.c_str());
		return true;
	}
	catch (const toml::parse_error& err)
	{
		std::cerr << "Cannot parse toml file: \"" << filepath.c_str() << "\": " << err.description() << std::endl;
		return false;
	}
}
